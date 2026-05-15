
#include "polonium/sockets/connection_handler.hpp"

#include <asm-generic/socket.h>
#include <sys/socket.h>

#include <atomic>
#include <csignal>
#include <expected>
#include <functional>
#include <memory>
#include <stdexcept>
#include <utility>

#include "polonium/app/polonium_logger.hpp"
#include "polonium/http/http.hpp"
#include "polonium/http/request_parser.hpp"
#include "polonium/http/response_serializer.hpp"
#include "polonium/routing/api_responses.hpp"
#include "polonium/routing/dispatcher.hpp"
#include "polonium/sockets/tcp_socket.hpp"
#include "polonium/thread_pool.hpp"

namespace {
std::atomic<bool> keep_running{true};
auto sigintHandler([[maybe_unused]] int /* unused */) {
    keep_running.store(false, std::memory_order_relaxed);
}
}  // namespace

// TODO: lessnik - Implement IPv6 support in future.
polonium::ConnectionHandler::ConnectionHandler(
    const polonium::PoloniumApiSettings& api_settings,
    const polonium::PoloinumSocketSettings& socket_settings,
    const polonium::PoloniumThreadPoolSettings& thread_pool_settings,
    polonium::Dispatcher& dispatcher)
    : ipv4_socket_(TcpIpv4Socket::createTcpIpv4Socket()),
      api_settings_(api_settings),
      thread_pool_settings_(thread_pool_settings),
      socket_settings_(socket_settings),
      thread_pool_(thread_pool_settings.getWorkersAmount()),
      dispatcher_(dispatcher),
      logger_(polonium::PoloniumLogger::getInstance()) {
    logger_->trace(__func__);
    logger_->info("Connection Handler initialization.");

    if (not ipv4_socket_.has_value()) {
        throw std::runtime_error(std::format(
            "{} says: Can not initialize TcpIpv4Socket object.", __func__));
    }
    server_fd_ = ipv4_socket_.value().getServerSocketFd();

    struct sigaction signal_action{};
    signal_action.sa_handler = sigintHandler;
    signal_action.sa_flags = 0;
    sigemptyset(&signal_action.sa_mask);
    sigaction(SIGINT, &signal_action, nullptr);

    auto bind_result =
        ipv4_socket_.value()
            .tcpBind(api_settings_.getHost(), api_settings_.getPort())
            .and_then(
                [this] -> std::expected<void, TcpIpv4Socket::TcpSocketErrors> {
                    return ipv4_socket_.value().tcpListen(
                        socket_settings_.getMaxBacklogSize());
                });
    if (not bind_result.has_value()) {
        keep_running.store(false, std::memory_order_relaxed);
    }
}
auto polonium::ConnectionHandler::acceptConnection() -> void {
    while (keep_running.load(std::memory_order_relaxed)) {
        auto accepted_pair = ipv4_socket_.value().tcpAccept();
        if (not accepted_pair.has_value()) {
            continue;
        }
        socket_fd client_fd = accepted_pair.value().first;
        struct timeval time_value = {.tv_sec = static_cast<__time_t>(
                                         socket_settings_.getTimeoutSeconds()),
                                     .tv_usec = 0};

        if (setsockopt(client_fd, SOL_SOCKET, SO_RCVTIMEO, &time_value,
                       sizeof(time_value)) < 0 or
            (setsockopt(client_fd, SOL_SOCKET, SO_SNDTIMEO, &time_value,
                        sizeof(time_value)) < 0)) {
            logger_->error("Can not set send or recv timeout seconds.");
        }

        thread_pool_.addTask(
            [this, client_fd]() -> void { handleConnection(client_fd); });
    }
    logger_->info("Got an interrupt. Preparing to shutdown...");
    thread_pool_.shutdown();
}

auto polonium::ConnectionHandler::handleConnection(socket_fd client_fd)
    -> void {
    HttpRequestParser request_parser;
    while (true) {
        auto buffer = ipv4_socket_.value().tcpRecv(client_fd);
        if (not buffer.has_value() or buffer.value().empty()) {
            close(client_fd);
            return;
        }
        HttpRequestParserStatus parser_status =
            request_parser.feed(buffer.value());
        std::string response_buffer;

        if (parser_status == HttpRequestParserStatus::Complete) {
            processParserStatusComplete(request_parser, response_buffer,
                                        client_fd);
        } else if (parser_status == HttpRequestParserStatus::NeedMore) {
            continue;
        } else {
            close(client_fd);
            return;
        }
    }
    close(client_fd);
}
auto polonium::ConnectionHandler::serializeResponseToBuffer(
    std::string& buffer, const HttpResponse& http_response) -> void {
    std::string response =
        HttpResponseSerializer(http_response).serializeResponse();
    buffer.resize(response.size());
    buffer.assign(response.begin(), response.end());
}
auto polonium::ConnectionHandler::processParserStatusComplete(
    HttpRequestParser& request_parser, std::string& response_buffer,
    socket_fd client_fd) const -> void {
    HttpRequest request = request_parser.getRequest();
    HandlerWithParams handler_with_params =
        dispatcher_.checkRoute(request.method, request.uri);

    if (!handler_with_params.handler) {
        // Endpoint not found. Serializing 404 (Not Found) response.
        logger_->info("Endpoint not found.");

        serializeResponseToBuffer(response_buffer,
                                  response_templates::get404ErrorResponse());
    } else {
        /* Accepted connection to the existing endpoint. Execute
         * user's function and serialize a response. Function
         * returns ApiResponse subclass object. Move headers and
         * body from the object.
         */
        logger_->info("Accepted connection to the existing endpoint.");
        if (not handler_with_params.path_params.empty()) {
            for (auto value : handler_with_params.path_params) {
                request.path_params.emplace(value);
            }
        }
        logger_->debug("Getting API response object.");
        std::shared_ptr<ApiResponse> api_response =
            handler_with_params.handler.value()(std::move(request));

        logger_->debug("Instantiate HttpResponse object.");
        HttpResponse http_response(*api_response);

        logger_->debug("Serializing response...");
        serializeResponseToBuffer(response_buffer, http_response);
    }
    logger_->debug("Send the response to the client.");

    auto send_result = ipv4_socket_.value().tcpSend(client_fd, response_buffer);
    if (not send_result.has_value()) {
        close(client_fd);
    }

    if (!request_parser.isKeepAlive()) {
        close(client_fd);
        return;
    }
    request_parser.reset();
}
