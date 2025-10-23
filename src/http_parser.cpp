
#include <cstddef>

#include "../include/http_parsing/http_parser.hpp"
#include "../include/http_parsing/http_parser_exceptions.hpp"
#include "http.hpp"



HttpResponseParser::HttpResponseParser(
    const std::vector<std::byte>& raw_response,
    HttpResponse& response
) : 
    raw_response(raw_response),
    response(response)
{
    http_parsed = &response;
    llhttp_settings_init(&settings);
    llhttp_init(&parser, HTTP_RESPONSE, &settings);
    // Set <parser.data> to manage current class instance in the handlers.
    parser.data = this;

    setCallbacks();
    parse();
}

HttpResponseParser::~HttpResponseParser() {
    llhttp_finish(&parser);
}

void HttpResponseParser::setCallbacks() {
    settings.on_message_begin = handler_on_message_begin;
    settings.on_status = handler_on_status;
    settings.on_status_complete = handler_on_status_complete;
    settings.on_protocol = handler_on_protocol;
    settings.on_version = handler_on_protocol_version;
    settings.on_header_field = handler_on_header_field;
    settings.on_header_value = handler_on_header_value;
    settings.on_header_value_complete = handler_on_header_value_complete;
    settings.on_body = handler_on_body;
}

void HttpResponseParser::clear() {
    response.status_code = 0;
    response.status_text.clear();
    response.protocol.clear();
    response.version.clear();
    response.headers.clear();
    response.body.clear();
    temporary_pair.first.clear();
    temporary_pair.second.clear();

}

void HttpResponseParser::parse() {
    enum llhttp_errno error_num = llhttp_execute(
        &parser, static_cast<const char*>(static_cast<const void*>(raw_response.data())), raw_response.size());    
    if(error_num != 0) {
        throw LlhttpBadErrno("There is an error while executing llhttp parser.", error_num);
    }
}


HttpRequestParser::HttpRequestParser(
    const std::vector<std::byte>& raw_request,
    HttpRequest& request
) : 
    raw_request(raw_request),
    request(request)
{
    http_parsed = &request;
    llhttp_settings_init(&settings);
    llhttp_init(&parser, HTTP_REQUEST, &settings);
    // Set <parser.data> to manage current class instance in the handlers.
    parser.data = this;

    setCallbacks();
    parse();
}

HttpRequestParser::~HttpRequestParser() {
    llhttp_finish(&parser);
}

void HttpRequestParser::setCallbacks() {
    settings.on_message_begin = handler_on_message_begin;
    settings.on_method = handler_on_method;
    settings.on_url = handler_on_uri;
    settings.on_protocol = handler_on_protocol;
    settings.on_version = handler_on_protocol_version;
    settings.on_header_field = handler_on_header_field;
    settings.on_header_value = handler_on_header_value;
    settings.on_header_value_complete = handler_on_header_value_complete;
    settings.on_body = handler_on_body;
}

void HttpRequestParser::parse() {
    enum llhttp_errno error_num = llhttp_execute(
        &parser, static_cast<const char*>(static_cast<const void*>(raw_request.data())), raw_request.size());    
    if(error_num != 0) {
        throw LlhttpBadErrno("There is an error while executing llhttp parser.", error_num);
    }
}

void HttpRequestParser::clear() {
    request.method.clear();
    request.uri.clear();
    request.protocol.clear();
    request.version.clear();
    request.headers.clear();
    request.body.clear();
    temporary_pair.first.clear();
    temporary_pair.second.clear();
}


// Callback methods realization.
// HttpResponseParser callback methods:
int HttpResponseParser::handler_on_status(llhttp_t* parser, const char* at, size_t length) {
    HttpResponseParser* self = static_cast<HttpResponseParser*>(parser->data);
    self->response.status_text.append(at, length);
    return 0;
}

int HttpResponseParser::handler_on_status_complete(llhttp_t* parser) {
    HttpResponseParser* self = static_cast<HttpResponseParser*>(parser->data);
    self->response.status_code = parser->status_code;
    return 0;
}

// HttpRequestParser callback methods:
int HttpRequestParser::handler_on_uri(llhttp_t* parser, const char* at, size_t length) {
    HttpRequestParser* self = static_cast<HttpRequestParser*>(parser->data);
    self->request.uri.append(at, length);
    return 0;
}

int HttpRequestParser::handler_on_method(llhttp_t* parser, const char* at, size_t length) {
    HttpRequestParser* self = static_cast<HttpRequestParser*>(parser->data);
    self->request.method.append(at, length);
    return 0;
}

// Base Parser callback methods:
int HttpParser::handler_on_message_begin(llhttp_t* parser) {
    HttpParser* self = static_cast<HttpParser*>(parser->data);
    // Reset all the fields in the class instance.
    self->clear();

    return 0;
}

int HttpParser::handler_on_protocol(llhttp_t* parser, const char* at, size_t length) {
    HttpParser* self = static_cast<HttpParser*>(parser->data);
    self->http_parsed->protocol.append(at, length);
    return 0;
}

int HttpParser::handler_on_protocol_version(llhttp_t* parser, const char* at, size_t length) {
    HttpParser* self = static_cast<HttpParser*>(parser->data);
    self->http_parsed->version.append(at, length);
    return 0;
}

int HttpParser::handler_on_header_field(llhttp_t* parser, const char* at, size_t length) {
    HttpParser* self = static_cast<HttpParser*>(parser->data);
    self->temporary_pair.first.append(at, length); 
    return 0;
}

int HttpParser::handler_on_header_value(llhttp_t* parser, const char* at, size_t length) {
    HttpParser* self = static_cast<HttpParser*>(parser->data);
    self->temporary_pair.second.append(at, length);  
    return 0;
}

int HttpParser::handler_on_header_value_complete(llhttp_t* parser) {
    HttpParser* self = static_cast<HttpParser*>(parser->data);
    self->http_parsed->headers.emplace(std::move(self->temporary_pair));  
    self->temporary_pair = {};
    return 0;
}

int HttpParser::handler_on_body(llhttp_t* parser, const char* at, size_t length) {
    HttpParser* self = static_cast<HttpParser*>(parser->data);
    self->http_parsed->body.append(at, length);
    return 0;
}
