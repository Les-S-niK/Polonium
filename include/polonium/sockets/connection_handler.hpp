#pragma once

#include "polonium/polonium_logger.hpp"
#include "polonium/routing/dispatcher.hpp"
#include "polonium/sockets/socket_config.hpp"
#include "polonium/sockets/tcp_socket.hpp"
#include "polonium/thread_pool.hpp"

// TODO: lessnik - Implement IPv6 support in future.

class ConnectionHandler {
   public:
    ConnectionHandler(std::string host, uint16_t port, Dispatcher& dispatcher,
                      uint32_t workers_amount) noexcept;
    ConnectionHandler(const ConnectionHandler&) = delete;
    ConnectionHandler(ConnectionHandler&&) = delete;
    auto operator=(const ConnectionHandler&) -> ConnectionHandler& = delete;
    auto operator=(ConnectionHandler&&) -> ConnectionHandler& = delete;
    ~ConnectionHandler() = default;

    auto acceptConnection() -> void;

   private:
    socket_fd server_fd_;
    TcpIpv4Socket ipv4_socket_;
    uint16_t port_;
    std::string host_;
    ThreadPool thread_pool_;
    Dispatcher& dispatcher_;
    PoloniumLogger* logger_;

    auto handleConnection(socket_fd client_fd) -> void;
};
