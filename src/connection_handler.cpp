
#include <cstdlib>
#include <string>
#include <thread>
#include <vector>
#include <sys/types.h>
// FIXME: REMOVE this include after development is complete:
#include <iostream>

#include "connection_handler.hpp"
#include "request_parser.hpp"
#include "http.hpp"
#include "tcp_socket.hpp"


// TODO: Implement IPv6 support in future.

void ConnectionHandler::acceptConnection() {
    // TODO: Make reasonable condition in future.
    while(true) {
        std::pair<socket_fd, struct sockaddr_in> accepted_pair = ipv4_socket.tcp_accept();
        // Create a new thread per connetion.
        // TODO: Add thread-pool.
        std::thread([this, accepted_pair](){
            handleConnection(accepted_pair.first, accepted_pair.second); 
        }).detach();
    }
}

void ConnectionHandler::handleConnection(int client_fd, struct sockaddr_in client_addr_in) {
    HttpRequestParser request_parser;

    while(true) {
        std::vector<char> buffer = ipv4_socket.tcp_recv(client_fd, socket_options::max_buffer_size);
        if(buffer.empty()) {
            close(client_fd);
            return;
        }
        HttpRequestParserStatus parser_status = request_parser.feed(
            std::string(buffer.begin(), buffer.begin() + buffer.size())
        );

        if(parser_status == HttpRequestParserStatus::Complete) {
            HttpRequest request = request_parser.getRequest();
            
            /*
            * FIXME: Delete cout's and the response string
            * in future and make response serialization logic.
            */ 
            std::cout
                << request.method << std::endl
                << request.getJson() << std::endl;

            std::string response = "HTTP/1.1 200 OK\r\nContent-Length: 3\r\n\r\nASD";
            std::vector<char> response_buffer(response.size());
            response_buffer.assign(response.begin(), response.end());
            ipv4_socket.tcp_send(client_fd, response_buffer);
            
            if(!request_parser.isKeepAlive()) {
                close(client_fd);
                return;
            }
            request_parser.reset();
        }
        else if(parser_status == HttpRequestParserStatus::NeedMore) {
            continue;
        }
    }
    close(client_fd);
}


