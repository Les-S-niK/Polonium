
#include <cassert>
#include <cstddef>
#include <cstdlib>
#include <string>
#include <thread>
#include <vector>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

// FIXME: REMOVE this include after development is complete:
#include <iostream>

#include "sockets/connection_sockets.hpp"
#include "sockets/socket_exceptions.hpp"
#include "http_parsing/http_parser.hpp"
#include "http.hpp"


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
    listen(fd, socket_options::max_backlog_size);

    // TODO: Make reasonable condition in future.
    while(true) {
        int client_fd = accept(fd, (struct sockaddr *) &client_addr_in, &client_socklen);
        closeSocketOnError(client_fd, "Can't correctly accept client_fd from the socket.");
        
        // Create a new thread per connetion.
        // TODO: Add thread-pool.
        std::thread([this, client_fd, client_addr_in](){
            handleConnection(client_fd, client_addr_in); 
            
            close(client_fd);
        }).detach();
    }
}

void Sockets::handleConnection(int client_fd, struct sockaddr_in client_addr_in)  {
    HttpRequestParser request_parser;
    std::vector<std::byte> buffer(socket_options::max_buffer_size);
    
    while(true) {
        ssize_t received_size = recv(client_fd, buffer.data(), buffer.size(), 0);
        closeSocketOnError(received_size, client_fd, "Can't correctly receive data.", client_addr_in);
        
        std::vector<std::byte> actual_data(buffer.begin(), buffer.begin() + received_size);
        request_parser.feedData(actual_data);

        if (request_parser.is_complete) {
            HttpRequest request = request_parser.getParsed();
            std::cout << request.method << std::endl;
            std::cout << request.uri << std::endl;
            std::cout << request.getJson() << std::endl;
            // Got request object. Need to check if URI in the routes table.
            
            if (request_parser.is_keep_alive) {
                continue;
            
            } else {
                break;
            }
        }
    }
}

inline void Sockets::bindSocket() {
    struct sockaddr_in addr_in;

    addr_in.sin_port = htons(port);
    addr_in.sin_family = AF_INET; 
    inet_aton(host.c_str(), &addr_in.sin_addr);
    
    closeSocketOnError(
        bind(fd, (const struct sockaddr *) &addr_in, sizeof(addr_in)),
        fd, "Can't correctly bind socket.", addr_in
    );
}

inline void Sockets::initSocket() {
    fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    closeSocketOnError(fd, fd, "Can't correctly create TCP/IPv4 socket");
}
