#pragma once

#include <netinet/in.h>
#include <sys/types.h>
#include <unistd.h>

#include <cstdint>
#include <string>
#include <utility>

#include "polonium/polonium_logger.hpp"
#include "polonium/routing/dispatcher.hpp"
#include "polonium/sockets/tcp_socket.hpp"
#include "polonium/thread_pool.hpp"

// TODO: lessnik - Implement IPv6 support in future.

namespace socket_options {
// Max backlog size for listen function. 10 is for tests. 128 or 4096 for
// production.
constexpr inline uint8_t max_backlog_size = 10;
constexpr inline size_t max_buffer_size = 8192;
}  // namespace socket_options

class ConnectionHandler {
   public:
    ConnectionHandler(std::string host, uint16_t port, Dispatcher& dispatcher,
                      uint32_t workers_amount)
        : port_(port),
          host_(std::move(host)),
          thread_pool_(workers_amount),
          dispatcher_(dispatcher),
          logger_(PoloniumLogger::getInstance()) {
        logger_->trace(__func__);
        logger_->info("Connection Handler initialization.");
        server_fd_ = ipv4_socket_.getServerSocketFd();
        ipv4_socket_.tcpBind(host_, port_);
        ipv4_socket_.tcpListen(socket_options::max_backlog_size);
    }

    void acceptConnection();

   private:
    int server_fd_;
    TcpIpv4Socket ipv4_socket_;
    uint16_t port_;
    std::string host_;
    ThreadPool thread_pool_;
    Dispatcher& dispatcher_;
    PoloniumLogger* logger_;

    void handleConnection(int client_fd);
};
