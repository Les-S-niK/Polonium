#pragma once

#include <cstdint>
#include <netinet/in.h>
#include <string>
#include <sys/types.h>


namespace socket_options {
    // Max backlog size for listen function. 10 is for tests. 128 or 4096 for production.
    constexpr inline uint8_t max_backlog_size = 10;
    constexpr inline size_t max_buffer_size = 8192;
}


class Sockets
{
    // TODO: Implement IPv6 support in future.
    public:
        Sockets(
            uint16_t port,
            std::string ipv4_host = "0.0.0.0"
        );

        void acceptConnection();

    
    private:
        int fd;
        uint16_t port;
        std::string host;

        inline void bindSocket();
        inline void initSocket();
        void handleConnection(int client_fd, struct sockaddr_in client_addr_in); 
};
