
#include <cstddef>
#include <string>
#include <thread>
#include <vector>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

// FIXME: DELETE this include:
#include <iostream>

#include "../include/connection_sockets.hpp"
#include "../include/http_parsing/http_parser.hpp"


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

void Sockets::acceptConnection() {
    struct sockaddr_in client_addr_in{};
    socklen_t client_socklen = sizeof(client_addr_in);
    listen(fd, max_backlog_size);
    // TODO: Make reasonable condition in future.
    while(true) {
        int client_fd = accept(fd, (struct sockaddr *) &client_addr_in, &client_socklen);
        if(client_fd == -1) {http://
            close(client_fd);
            // TODO: Make exception class and throw it.
            throw "Can't get client fd";
        }
        // TODO: Add recv, and send function calls.
        // TODO: Make thread-per-connection server model.
        std::thread([client_fd](){
            std::vector<std::byte> buffer(8192);
            
            size_t recieved_size = recv(client_fd, buffer.data(), buffer.size(), 0);
            if(recieved_size == -1) {
                // TODO: Make exception class and throw it here.
                throw "Can't recieve data.";
            }
            buffer.resize(recieved_size);
            
            HttpRequestParser request_parser(buffer);
            size_t content_length = 0;
            if(request_parser.headers.find("Content-Length") != request_parser.headers.end()) {
                content_length = std::stoul(request_parser.headers.at("Content-Length"));
            }
            // TODO: Check content length and compare it with the current body length.
            close(client_fd);
        }).detach();
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

