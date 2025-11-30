
#include <cstdlib>
#include <functional>
#include <string>
#include <thread>
#include <unistd.h>
#include <sys/types.h>

#include "connection_handler.hpp"
#include "http/http.hpp"
#include "http/request_parser.hpp"
#include "http/response_serializer.hpp"
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
        std::string buffer;
        buffer.resize(socket_options::max_buffer_size);

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

            std::string response_buffer;
            logger_.info("Before handler check");
            if(!handler) {
                logger_.info("Endpoint did not find.");

                HttpResponse http_response;
                http_response.protocol = http_options::protocol;
                http_response.version = http_options::version_1_1;
                http_response.status_code = status_codes::not_found_404.first;
                http_response.status_text = status_codes::not_found_404.second;
                http_response.headers[http_headers::content_length] = '0';
                
                // TODO: Add Response serialization class.
                HttpResponseSerializer serializer(logger_, http_response);
                std::string response = serializer.serializeResponse();

                response_buffer.resize(response.size());
                response_buffer.assign(response.begin(), response.end());
            }
            else {
                logger_.info("Accepted connection to the existing endpoint.");
                json json_result = handler.value()();
                std::string dumped = json_result.dump();
                HttpResponse http_response;
                
                http_response.protocol = http_options::protocol;
                http_response.version = http_options::version_1_1;
                http_response.status_code = status_codes::ok_200.first;
                http_response.status_text = status_codes::ok_200.second;
                http_response.headers[http_headers::content_length] = std::to_string(dumped.size());
                http_response.headers[http_headers::content_type] = "application/json; charset=utf-8";
                http_response.body = dumped;

                HttpResponseSerializer serializer(logger_, http_response);
                std::string response = serializer.serializeResponse();

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
