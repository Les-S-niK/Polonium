
#include <array>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "../include/connection_sockets.hpp"


// TODO: Implement IPv6 support in future.
Sockets::Sockets(
    uint16_t port,
    std::string ipv4_host
) : 
    port(port),
    host(ipv4_host)
{
    initSocket();
    bindSocket();
}

std::vector<std::byte> Sockets::acceptConnection() {
    struct sockaddr_in client_addr_in;
    socklen_t client_socklen = sizeof(client_addr_in);
    listen(fd, max_backlog_size);
    // TODO: Make reasonable condition in future.
    while(true) {
        client_fd = accept(fd, (struct sockaddr *) &client_addr_in, &client_socklen);
        if(client_fd == -1) {
            close(client_fd);
            // TODO: Make exception class and throw it.
            throw "Can't get client fd";
        }
        // TODO: Add recv, and send function calls.
        // This code is a very simple parser test.
        std::vector<std::byte> buffer;
        std::array<std::byte, 8192> arr{}; 
        recv(client_fd, &arr, arr.size(), 0);
        buffer.insert(buffer.end(), arr.begin(), arr.end());
        close(client_fd);

        return buffer;
    }
}

inline void Sockets::bindSocket() {
    struct sockaddr_in addr_in;

    addr_in.sin_port = htons(port);
    addr_in.sin_family = AF_INET; 
    inet_aton(host.c_str(), &addr_in.sin_addr);

    if(bind(fd, (const struct sockaddr *) &addr_in, sizeof(addr_in)) == -1) {
        close(fd);
        // TODO: Make exception class and throw it.
        throw "Can't bind socket.";
    }
}

inline void Sockets::initSocket() {
    fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(fd == -1) {
        close(fd);
        // TODO: Make exception class and throw it.
        throw "Can't create TCP socket.";
    } 
}

