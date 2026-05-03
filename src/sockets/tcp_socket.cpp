#include "polonium/sockets/tcp_socket.hpp"

#include <asm-generic/socket.h>
#include <sys/socket.h>
#include <unistd.h>

#include <array>
#include <format>
#include <iterator>
#include <system_error>

#include "polonium/sockets/socket_exceptions.hpp"

TcpIpv4Socket::TcpIpv4Socket()
    : logger_(PoloniumLogger::getInstance()),
      server_fd_(socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) {
    logger_->trace(__func__);
    logger_->info("TCP/IPv4 socket initialization.");

    if (server_fd_ == -1) {
        logAndThrowException_(exception_messages::tcp_socket);
    }
    logger_->debug("Created TCP/IPv4 socket.");
}

TcpIpv4Socket::~TcpIpv4Socket() {
    logger_->trace(__func__);
    if (server_fd_ != -1) {
        shutdown(server_fd_, SHUT_RDWR);
        close(server_fd_);
    }
    logger_->debug("Closed TCP/IPv4 socket.");
}

[[nodiscard]] auto TcpIpv4Socket::getServerSocketFd() const -> socket_fd {
    logger_->trace(__func__);
    return server_fd_;
}

auto TcpIpv4Socket::tcpBind(const std::string& host, const uint16_t& port)
    -> void {
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
        logAndThrowException_(
            "Can not set socket option to reuse ADDR or PORT.");
    }

    if (bind(server_fd_, reinterpret_cast<struct sockaddr*>(&server_addr),
             sizeof(server_addr)) == -1) {
        logAndThrowException_(exception_messages::tcp_bind);
    }
    logger_->debug("TCP/IPv4 socket binded.");
}

auto TcpIpv4Socket::tcpListen(const int& max_backlog_size) -> void {
    logger_->trace(__func__);
    if (listen(server_fd_, max_backlog_size) == -1) {
        logAndThrowException_(exception_messages::tcp_listen);
    }
    logger_->debug(std::format("Listen. Max backlog size: {} connections",
                               max_backlog_size));
}

auto TcpIpv4Socket::tcpAccept() -> std::pair<socket_fd, struct sockaddr_in> {
    logger_->trace(__func__);
    struct sockaddr_in client_addr{};
    socklen_t client_addr_len = sizeof(client_addr);
    socket_fd client_fd =
        accept(server_fd_, reinterpret_cast<struct sockaddr*>(&client_addr),
               &client_addr_len);

    if (client_fd == -1) {
        logAndThrowException_(exception_messages::tcp_accept);
    }
    std::array<char, praddr4_len> protoaddr{};
    inet_ntop(AF_INET, &client_addr.sin_addr, protoaddr.data(),
              protoaddr.size());

    logger_->debug(std::format(
        "Accepted TCP/IPv4 connection.\nClient addr: {}\nClient port: {}",
        protoaddr.data(), ntohs(client_addr.sin_port)));

    return std::make_pair(client_fd, client_addr);
}

auto TcpIpv4Socket::tcpRecv(const socket_fd& client_fd, const int flags)
    -> std::string {
    logger_->trace(__func__);
    std::array<char, socket_options::max_buffer_size> buffer{};

    ssize_t recieved_size =
        recv(client_fd, buffer.data(), buffer.size(), flags);

    if (recieved_size == -1) {
        logAndThrowException_(exception_messages::tcp_recv);
    } else if (recieved_size == 0) {
        return {};
    }
    logger_->debug(
        std::format("Recieved buffer with the size: {}", recieved_size));
    return {buffer.begin(), std::next(buffer.begin(), recieved_size)};
}

auto TcpIpv4Socket::tcpSend(const socket_fd& client_fd, std::string_view buffer,
                            const int flags) -> void {
    logger_->trace(__func__);
    if (send(client_fd, buffer.data(), buffer.size(), flags) == -1) {
        logAndThrowException_(exception_messages::tcp_send);
    }
    logger_->debug(std::format("Sent buffer with the size: {}", buffer.size()));
}

auto TcpIpv4Socket::logAndThrowException_(std::string_view message) const
    -> void {
    std::string final_message = std::format(
        "{}. Errno: {}", message,
        (std::error_code(errno, std::generic_category()).message()));
    logger_->critical(final_message);
    throw socket_exception(final_message);
}
