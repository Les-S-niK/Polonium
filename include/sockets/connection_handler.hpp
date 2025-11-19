#pragma once

#include <cstdint>
#include <netinet/in.h>
#include <string>
#include <sys/types.h>
#include <unistd.h>

// #include "http.hpp"
#include "polonium_logger.hpp"
#include "tcp_socket.hpp"
#include "dispatcher.hpp"


// TODO: Implement IPv6 support in future.


namespace socket_options {
    // Max backlog size for listen function. 10 is for tests. 128 or 4096 for production.
    constexpr inline uint8_t max_backlog_size = 10;
    constexpr inline size_t max_buffer_size = 8192;
}


class ConnectionHandler
{
    public:
        ConnectionHandler(
            std::string host,
            uint16_t port,
            PoloniumLogger& logger,
            Dispatcher& dispatcher
        ) :
            host_(host),
            port_(port),
            logger_(logger),
            ipv4_socket_(logger),
            dispatcher_(dispatcher)
        {
            logger_.trace(__func__);
            logger.info("Connection Handler initialization.");
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
        Dispatcher& dispatcher_;
        PoloniumLogger& logger_;

        void handleConnection(int client_fd, struct sockaddr_in client_addr_in); 
};
