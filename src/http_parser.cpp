
#include <iostream>

#include "../include/http_parser.hpp"


HttpRequestsParser::HttpRequestsParser(const std::string& request) : request(request) {
    llhttp_settings_init(&settings);
    llhttp_init(&parser, HTTP_REQUEST, &settings);
    // Set <parser.data> to manage current class instance in the handlers.
    parser.data = this;

    setCallbacks();
    parse();
}

void HttpRequestsParser::setCallbacks() {
    settings.on_message_begin = handler_on_message_begin;
    settings.on_url = handler_on_url;
    settings.on_method = handler_on_method;
    settings.on_protocol = handler_on_protocol;
    settings.on_version = handler_on_version;
    settings.on_header_field = handler_on_header_field;
    settings.on_header_value = handler_on_header_value;
    settings.on_header_value_complete = handler_on_header_value_complete;
    settings.on_body = handler_on_body;
}

void HttpRequestsParser::parse() {
    enum llhttp_errno error_num = llhttp_execute(
        &parser, request.c_str(), request.length());    
    if(error_num != 0) {
        // TODO: Throw an exception class here in future.
        std::cerr << "Error code: " << error_num << std::endl;
        throw "The error code is bad.";
    }
}


// Callback methods realization.
int HttpRequestsParser::handler_on_message_begin(llhttp_t* parser) {
    std::cout << "Begin..." << std::endl;
    return 0;
}

int HttpRequestsParser::handler_on_url(llhttp_t* parser, const char* at, size_t length) {
    HttpRequestsParser* self = static_cast<HttpRequestsParser*>(parser->data);
    self->url = std::string(at, length);
    return 0;
}

int HttpRequestsParser::handler_on_method(llhttp_t* parser, const char* at, size_t length) {
    HttpRequestsParser* self = static_cast<HttpRequestsParser*>(parser->data);
    self->method = std::string(at, length);
    return 0;
}

int HttpRequestsParser::handler_on_protocol(llhttp_t* parser, const char* at, size_t length) {
    HttpRequestsParser* self = static_cast<HttpRequestsParser*>(parser->data);
    self->protocol = std::string(at, length);
    return 0;
}

int HttpRequestsParser::handler_on_version(llhttp_t* parser, const char* at, size_t length) {
    HttpRequestsParser* self = static_cast<HttpRequestsParser*>(parser->data);
    self->version = std::string(at, length);
    return 0;
}

int HttpRequestsParser::handler_on_header_field(llhttp_t* parser, const char* at, size_t length) {
    HttpRequestsParser* self = static_cast<HttpRequestsParser*>(parser->data);
    self->temporary_pair.first = std::string(at, length); 
    return 0;
}

int HttpRequestsParser::handler_on_header_value(llhttp_t* parser, const char* at, size_t length) {
    HttpRequestsParser* self = static_cast<HttpRequestsParser*>(parser->data);
    self->temporary_pair.second = std::string(at, length);  
    return 0;
}

int HttpRequestsParser::handler_on_header_value_complete(llhttp_t* parser) {
    HttpRequestsParser* self = static_cast<HttpRequestsParser*>(parser->data);
    self->headers.emplace(self->temporary_pair);  
    return 0;
}

int HttpRequestsParser::handler_on_body(llhttp_t* parser, const char* at, size_t length) {
    HttpRequestsParser* self = static_cast<HttpRequestsParser*>(parser->data);
    self->body = std::string(at, length);
    return 0;
}
