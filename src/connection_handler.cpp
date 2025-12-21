
#include <cstdlib>
#include <functional>
#include <string>
#include <thread>
#include <unistd.h>
#include <sys/types.h>

#include "connection_handler.hpp"
#include "api_responses.hpp"
#include "dispatcher.hpp"
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
            std::optional<endpoint_handler> handler = dispatcher_.checkRoute(request.method, request.uri);
            uri_path_params params_type_and_value = dispatcher_.findAllPathParams(request.uri);
            for(auto& a : params_type_and_value) {
                logger_.warning(a.first);
                logger_.warning(a.second);
            }

            std::string response_buffer;
            if(!handler) {
                logger_.info("Endpoint not found.");
                std::string json_str = R"({"status": 404, "message": "Endpoint not found."})";

                HttpResponse http_response;
                http_response.protocol = http_options::protocol;
                http_response.version = http_options::version_1_1;
                http_response.status_code = status_codes::not_found_404.first;
                http_response.status_text = status_codes::not_found_404.second;
                http_response.headers[http_headers::content_length] = std::to_string(json_str.size());
                http_response.headers[http_headers::content_type] = "application/json; charset=utf-8";
                http_response.body = json_str;
                
                // TODO: Add Response serialization class.
                HttpResponseSerializer serializer(logger_, http_response);
                std::string response = serializer.serializeResponse();

                response_buffer.resize(response.size());
                response_buffer.assign(response.begin(), response.end());
            }
            else {
                logger_.info("Accepted connection to the existing endpoint.");
                ApiResponse api_response = handler.value()(request);
                std::string dumped = api_response.content;
                
                HttpResponse http_response;
                http_response.protocol = http_options::protocol;
                http_response.version = http_options::version_1_1;
                http_response.status_code = api_response.status_code.first;
                http_response.status_text = api_response.status_code.second;
                http_response.headers[http_headers::content_length] = std::to_string(dumped.size());
                http_response.headers[http_headers::content_type] = "application/json; charset=utf-8";
                for(const std::pair<std::string, std::string>& header : api_response.headers)
                    http_response.headers.insert(header);
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
