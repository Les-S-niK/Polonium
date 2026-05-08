#pragma once

#include <expected>

#include "polonium/polonium_logger.hpp"
#include "polonium/routing/dispatcher.hpp"
#include "polonium/sockets/socket_config.hpp"
#include "polonium/sockets/tcp_socket.hpp"
#include "polonium/thread_pool.hpp"

// TODO: lessnik - Implement IPv6 support in future.

class ConnectionHandler {
   public:
    ConnectionHandler(std::string host, uint16_t port, Dispatcher& dispatcher,
                      uint32_t workers_amount);
    ConnectionHandler(const ConnectionHandler& other) noexcept = delete;
    ConnectionHandler(ConnectionHandler&& other) noexcept = delete;
    auto operator=(const ConnectionHandler& other) noexcept
        -> ConnectionHandler& = delete;
    auto operator=(ConnectionHandler&& other) noexcept
        -> ConnectionHandler& = delete;
    ~ConnectionHandler() = default;

    auto acceptConnection() -> void;

   private:
    socket_fd server_fd_;
    std::expected<TcpIpv4Socket, TcpIpv4Socket::TcpSocketErrors> ipv4_socket_;
    uint16_t port_;
    std::string host_;
    ThreadPool thread_pool_;
    Dispatcher& dispatcher_;
    PoloniumLogger* logger_;

    auto handleConnection(socket_fd client_fd) -> void;
};
