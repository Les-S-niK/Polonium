#pragma once

#include <expected>

#include "polonium/app/polonium_config.hpp"
#include "polonium/app/polonium_logger.hpp"
#include "polonium/http/http_actions.hpp"
#include "polonium/http/request_parser.hpp"
#include "polonium/http/response_serializer.hpp"
#include "polonium/routing/dispatcher.hpp"
#include "polonium/sockets/tcp_socket.hpp"
#include "polonium/thread_pool.hpp"

// TODO: lessnik - Implement IPv6 support in future.

namespace polonium {
class ConnectionHandler {
   public:
    ConnectionHandler(
        const polonium::PoloniumApiSettings& api_settings,
        const polonium::PoloinumSocketSettings& socket_settings,
        const polonium::PoloniumThreadPoolSettings& thread_pool_settings,
        polonium::Dispatcher& dispatcher);
    ConnectionHandler(const ConnectionHandler& other) noexcept = delete;
    ConnectionHandler(ConnectionHandler&& other) noexcept = delete;
    auto operator=(const ConnectionHandler& other) noexcept
        -> ConnectionHandler& = delete;
    auto operator=(ConnectionHandler&& other) noexcept
        -> ConnectionHandler& = delete;
    ~ConnectionHandler() = default;

    auto acceptConnection() -> void;

   private:
    auto handleConnection(socket_fd client_fd) -> void;
    static auto serializeResponseToBuffer(std::string& buffer,
                                          const HttpResponse& http_response)
        -> void;
    auto processParserStatusComplete(HttpRequestParser& request_parser,
                                     std::string& response_buffer,
                                     socket_fd client_fd) const -> void;

    socket_fd server_fd_;
    std::expected<TcpIpv4Socket, TcpIpv4Socket::TcpSocketErrors> ipv4_socket_;
    polonium::PoloniumApiSettings api_settings_;
    polonium::PoloniumThreadPoolSettings thread_pool_settings_;
    polonium::PoloinumSocketSettings socket_settings_;
    ThreadPool thread_pool_;
    Dispatcher& dispatcher_;
    polonium::PoloniumLogger* logger_;
};
}  // namespace polonium
