
#include <functional>
#include <thread>

#include "connection_handler.hpp"
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
        catch(socket_exception& exception) { close(client_fd); return; }
        if(buffer.empty()) { close(client_fd); return; }

        HttpRequestParserStatus parser_status = request_parser.feed(buffer);

        if(parser_status == HttpRequestParserStatus::Complete) {
            HttpRequest request = request_parser.getRequest();
            HandlerWithParams handler_with_params = dispatcher_.checkRoute(request.method, request.uri);
            std::string response_buffer;

            if(!handler_with_params.handler) {
                // Endpoint not found. Serializing 404 (Not Found) response.
                logger_.info("Endpoint not found.");
                
                std::string response =  HttpResponseSerializer(logger_, response_templates::error_404_response).serializeResponse();
                response_buffer.resize(response.size());
                response_buffer.assign(response.begin(), response.end());
            }
            else {
                /* Accepted connection to the existing endpoint. Execute user's function and serialize a response.
                * Function returns ApiResponse subclass object. Move headers and body from the object.
                */ 
                logger_.info("Accepted connection to the existing endpoint.");
                if(!handler_with_params.path_params.empty()) {
                    for(auto value : handler_with_params.path_params)
                        request.path_params.emplace(value);
                }
                auto api_response = handler_with_params.handler.value()(std::move(request));                
                
                HttpResponse http_response(http_options::protocol, http_options::version_1_1, api_response->status_code);
                http_response.headers[http_headers::content_length] = std::to_string(api_response->getContent().size());
                http_response.headers[http_headers::content_type] = "application/json; charset=utf-8";
                for(const std::pair<std::string, std::string>&& header : api_response->headers)
                    http_response.headers.insert(std::move(header));
                http_response.body = std::move(api_response->getContent());

                std::string response = HttpResponseSerializer(logger_, http_response).serializeResponse();
                response_buffer.resize(response.size());
                response_buffer.assign(response.begin(), response.end());
            }
            try { ipv4_socket_.tcpSend(client_fd, response_buffer); }
            catch(socket_exception&) { close(client_fd); }
            
            if(!request_parser.isKeepAlive()) { close(client_fd); return; }
            request_parser.reset();
        }
        else if(parser_status == HttpRequestParserStatus::NeedMore) continue;
        else { close(client_fd); return; }
    }
    close(client_fd);
}
