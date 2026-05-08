#include "polonium/sockets/tcp_socket.hpp"

#include <arpa/inet.h>
#include <asm-generic/socket.h>
#include <sys/socket.h>
#include <unistd.h>

#include <array>
#include <cerrno>
#include <expected>
#include <format>
#include <iterator>
#include <stdexcept>
#include <system_error>
#include <utility>

#include "polonium/app/polonium_config.hpp"
#include "polonium/app/polonium_logger.hpp"

TcpIpv4Socket::TcpIpv4Socket(socket_fd server_fd)
    : server_fd_{server_fd}, logger_(polonium::PoloniumLogger::getInstance()) {
    logger_->trace(__func__);
    logger_->info("TCP/IPv4 socket initialization.");
}

TcpIpv4Socket::TcpIpv4Socket(TcpIpv4Socket&& other) noexcept
    : server_fd_(std::exchange(other.server_fd_, -1)), logger_(other.logger_) {}

auto TcpIpv4Socket::operator=(TcpIpv4Socket&& other) noexcept
    -> TcpIpv4Socket& {
    if (this != &other) {
        if (server_fd_ != -1) {
            close(server_fd_);
        }
        server_fd_ = std::exchange(other.server_fd_, -1);
        logger_ = other.logger_;
    }
    return *this;
}

TcpIpv4Socket::~TcpIpv4Socket() {
    logger_->trace(__func__);
    if (server_fd_ != -1) {
        shutdown(server_fd_, SHUT_RDWR);
        close(server_fd_);
    }
    logger_->debug("Closed TCP/IPv4 socket.");
}

[[nodiscard]] auto TcpIpv4Socket::createTcpIpv4Socket() noexcept
    -> std::expected<TcpIpv4Socket, TcpSocketErrors> {
    polonium::PoloniumLogger* logger = nullptr;
    try {
        logger = polonium::PoloniumLogger::getInstance();
    } catch (const std::runtime_error&) {
        return std::unexpected(TcpSocketErrors::TcpSocketInitError);
    }

    socket_fd server_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (server_fd == -1) {
        logger->critical(exception_messages::tcp_socket);
        return std::unexpected(TcpSocketErrors::TcpSocketInitError);
    }
    TcpIpv4Socket tcp_socket{server_fd};
    logger->info("Successfully created TCP/IPv4 socket.");
    return tcp_socket;
}

auto TcpIpv4Socket::getServerSocketFd() const -> socket_fd {
    logger_->trace(__func__);
    return server_fd_;
}

auto TcpIpv4Socket::tcpBind(const std::string& host, const uint16_t& port) const
    -> std::expected<void, TcpSocketErrors> {
    logger_->trace(__func__);

    struct sockaddr_in server_addr{};
    inet_aton(host.c_str(), &server_addr.sin_addr);
    server_addr.sin_port = htons(port);

    server_addr.sin_family = AF_INET;
    logger_->debug(std::format(
        "Try binding TCP/IPv4 socket.\nHost: {}\nPort: {}", host, port));

    constexpr int enable = 1;
    if (setsockopt(server_fd_, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) <
            0 or
        setsockopt(server_fd_, SOL_SOCKET, SO_REUSEPORT, &enable, sizeof(int)) <
            0) {
        logSocketError_(exception_messages::tcp_bind_sockopt_error,
                        &polonium::PoloniumLogger::warning);
    }

    if (bind(server_fd_, reinterpret_cast<struct sockaddr*>(&server_addr),
             sizeof(server_addr)) == -1) {
        logSocketError_(exception_messages::tcp_bind,
                        &polonium::PoloniumLogger::critical);
        return std::unexpected(TcpSocketErrors::TcpSocketBindError);
    }
    logger_->debug("TCP/IPv4 socket binded.");
    return {};
}

auto TcpIpv4Socket::tcpListen(const int& max_backlog_size) const
    -> std::expected<void, TcpSocketErrors> {
    logger_->trace(__func__);
    if (listen(server_fd_, max_backlog_size) == -1) {
        logSocketError_(exception_messages::tcp_listen,
                        &polonium::PoloniumLogger::critical);
        return std::unexpected(TcpSocketErrors::TcpSocketListenErorr);
    }
    logger_->debug(std::format("Listen. Max backlog size: {} connections",
                               max_backlog_size));
    return {};
}

auto TcpIpv4Socket::tcpAccept() const
    -> std::expected<std::pair<socket_fd, struct sockaddr_in>,
                     TcpSocketErrors> {
    logger_->trace(__func__);
    struct sockaddr_in client_addr{};
    socklen_t client_addr_len = sizeof(client_addr);
    socket_fd client_fd =
        accept(server_fd_, reinterpret_cast<struct sockaddr*>(&client_addr),
               &client_addr_len);

    if (client_fd == -1) {
        if (int errno_core =
                std::error_code(errno, std::generic_category()).value();
            errno_core == EINTR) {
            logSocketError_(exception_messages::tcp_accept_interrupted,
                            &polonium::PoloniumLogger::warning);
            return std::unexpected(
                TcpSocketErrors::TcpSocketAcceptInterruptError);
        }
        logSocketError_(exception_messages::tcp_accept,
                        &polonium::PoloniumLogger::error);
        return std::unexpected(TcpSocketErrors::TcpSocketAcceptInvalidFdError);
    }
    std::array<char, praddr4_len> protoaddr{};
    inet_ntop(AF_INET, &client_addr.sin_addr, protoaddr.data(),
              protoaddr.size());

    logger_->debug(std::format(
        "Accepted TCP/IPv4 connection.\nClient addr: {}\nClient port: {}",
        protoaddr.data(), ntohs(client_addr.sin_port)));

    return std::make_pair(client_fd, client_addr);
}

auto TcpIpv4Socket::tcpRecv(const socket_fd& client_fd, const int flags) const
    -> std::expected<std::string, TcpSocketErrors> {
    logger_->trace(__func__);
    constexpr uint16_t max_buffer_size = 8192;
    std::array<char, max_buffer_size> buffer{};

    ssize_t recieved_size =
        recv(client_fd, buffer.data(), buffer.size(), flags);

    if (recieved_size == -1) {
        if (int errno_code =
                std::error_code(errno, std::generic_category()).value();
            errno_code == EAGAIN or errno_code == EWOULDBLOCK) {
            logSocketError_(exception_messages::tcp_recv_timeout,
                            &polonium::PoloniumLogger::debug);
            return std::unexpected(TcpSocketErrors::TcpSocketRecvTimeoutError);
        }
        logSocketError_(exception_messages::tcp_recv,
                        &polonium::PoloniumLogger::error);
        return std::unexpected(TcpSocketErrors::TcpSocketRecvInvalidSizeError);
    }
    if (recieved_size == 0) {
        return std::string{};
    }
    logger_->debug(
        std::format("Recieved buffer with the size: {}", recieved_size));
    return std::string{buffer.begin(),
                       std::next(buffer.begin(), recieved_size)};
}

auto TcpIpv4Socket::tcpSend(const socket_fd& client_fd, std::string_view buffer,
                            const int flags) const
    -> std::expected<ssize_t, TcpSocketErrors> {
    logger_->trace(__func__);
    ssize_t sent_size = send(client_fd, buffer.data(), buffer.size(), flags);

    if (sent_size == -1) {
        logSocketError_(exception_messages::tcp_send,
                        &polonium::PoloniumLogger::error);
        return std::unexpected(TcpSocketErrors::TcpSocketSendError);
    }
    logger_->debug(std::format("Sent buffer with the size: {}", buffer.size()));
    return sent_size;
}
