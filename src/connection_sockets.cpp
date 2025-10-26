
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
    listen(fd, max_backlog_size);

    // TODO: Make reasonable condition in future.
    while(true) {
        int client_fd = accept(fd, (struct sockaddr *) &client_addr_in, &client_socklen);
        closeSocketOnError(client_fd, "Can't correctly accept client_fd from the socket.");
        
        // Create a new thread per connetion.
        std::thread([this, client_fd, client_addr_in](){
            std::vector<std::byte> buffer(max_buffer_size);
            
            ssize_t received_size = recv(client_fd, buffer.data(), buffer.size(), 0);
            closeSocketOnError(received_size, client_fd, "Can't correctly receive data from the client socket.", client_addr_in);
            buffer.resize(received_size);

            HttpRequest request = HttpRequestParser(std::move(buffer)).getParsed();
            size_t content_length = 0;
            if(request.headers.find("Content-Length") != request.headers.end()) {
                content_length = std::stoi(request.headers.at("Content-Length"));
            }

            while(request.body.length() < content_length) {
                // TODO: Receive other body parts
            }
            /*
             * HTTP packet received. Now we need to check if endpoint of the packet is in the 
             * routes table.
             */
            
            close(client_fd);
        }).detach();
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

