
#include <cstdlib>
#include <functional>
#include <string>
#include <thread>
#include <unistd.h>
#include <vector>
#include <sys/types.h>
// FIXME: REMOVE this include after development is complete:
#include <iostream>

#include "connection_handler.hpp"
#include "request_parser.hpp"
#include "socket_exceptions.hpp"


// TODO: Implement IPv6 support in future.

void ConnectionHandler::acceptConnection() {
    // TODO: Make reasonable condition in future.
    while(true) {
        std::pair<socket_fd, struct sockaddr_in> accepted_pair = ipv4_socket_.tcpAccept();
        // Create a new thread per connetion.
        // TODO: Add thread-pool.
        std::thread([this, accepted_pair](){
            handleConnection(accepted_pair.first, accepted_pair.second); 
        }).detach();
    }
}

void ConnectionHandler::handleConnection(int client_fd, struct sockaddr_in client_addr_in) {
    HttpRequestParser request_parser(logger_);

    while(true) {
        std::vector<char> buffer(socket_options::max_buffer_size);

        try { buffer = ipv4_socket_.tcpRecv(client_fd, socket_options::max_buffer_size); }
        catch(socket_exception& exception) {
            close(client_fd);
            return;
        }
        if(buffer.empty()) {
            close(client_fd);
            return;
        }

        HttpRequestParserStatus parser_status = request_parser.feed(buffer);

        if(parser_status == HttpRequestParserStatus::Complete) {
            HttpRequest request = request_parser.getRequest();
            std::optional<std::function<json()>> handler = dispatcher_.checkRoute(request.method, request.uri);

            std::vector<char> response_buffer;
            if(!handler) {
                logger_.info("Endpoint did not found.");
                // TODO: Add Response serialization class.
                std::string response = "HTTP/1.1 404 NOT FOUND\r\nContent-Length: 0\r\n\r\n";
                response_buffer.resize(response.size());
                response_buffer.assign(response.begin(), response.end());
            }
            else {
                logger_.info("Accepted connection to the existing endpoint.");
                // TODO: Add Response serialization class.
                json json_result = handler.value()();
                std::cout << json_result.dump() << std::endl;
                std::string dumped = json_result.dump();
                std::string response = 
                    "HTTP/1.1 200 OK\r\n"
                    "Content-Length: " + std::to_string(dumped.size()) + "\r\n"
                    "Content-Type: application/json; charset=utf-8\r\n"
                    "\r\n" +
                    dumped;
                response_buffer.resize(response.size());
                response_buffer.assign(response.begin(), response.end());
            }
            try { ipv4_socket_.tcpSend(client_fd, response_buffer); }
            catch(socket_exception&) { close(client_fd); }
            
            if(!request_parser.isKeepAlive()) {
                close(client_fd);
                return;
            }
            request_parser.reset();
        }
        else if(parser_status == HttpRequestParserStatus::NeedMore) {
            continue;
        }
        else {
            close(client_fd);
            return;
        }
    }
    close(client_fd);
}


