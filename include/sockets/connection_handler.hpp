#pragma once

#include <cstdint>
#include <netinet/in.h>
#include <string>
#include <sys/types.h>
#include <unistd.h>
#include <iostream>

#include "http.hpp"
#include "socket_exceptions.hpp"
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
        Dispatcher dispatcher;

        ConnectionHandler(
            std::string host,
            uint16_t port
        ) :
            host(host),
            port(port) 
        {
            server_fd = ipv4_socket.getServerSocketFd();
            try {
                ipv4_socket.tcp_bind(host, port);
                ipv4_socket.tcp_listen(socket_options::max_backlog_size);
            }
            catch(socket_exception& exception) {
                std::cout << exception.what() << std::endl;
            }
        }

        void acceptConnection();

    
    private:
        int server_fd;
        TcpIpv4Socket ipv4_socket;
        uint16_t port;
        std::string host;

        void handleConnection(int client_fd, struct sockaddr_in client_addr_in); 
};
