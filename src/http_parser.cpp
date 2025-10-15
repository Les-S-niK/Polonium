
#include "../include/http_parsing/http_parser.hpp"
#include "../include/http_parsing/http_parser_exceptions.hpp"

// TODO: Add getJson method for HttpParser and make it's implementation in the .cpp file.
/**
 * @brief Construct a new Http Response Parser object. 
 * This class parses HTTP responses.
 * 
 * @param response
 */
HttpResponseParser::HttpResponseParser(const std::string& response) : response(response) {
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
    settings.on_message_begin = callbacks::handler_on_message_begin;
    settings.on_status = callbacks::handler_on_status;
    settings.on_status_complete = callbacks::handler_on_status_complete;
    settings.on_protocol = callbacks::handler_on_protocol;
    settings.on_version = callbacks::handler_on_version;
    settings.on_header_field = callbacks::handler_on_header_field;
    settings.on_header_value = callbacks::handler_on_header_value;
    settings.on_header_value_complete = callbacks::handler_on_header_value_complete;
    settings.on_body = callbacks::handler_on_body;
}

void HttpResponseParser::clear() {
    status_code = 0;
    status_text.clear();
    protocol.clear();
    version.clear();
    headers.clear();
    body.clear();
    temporary_pair.first.clear();
    temporary_pair.second.clear();
}

void HttpResponseParser::parse() {
    enum llhttp_errno error_num = llhttp_execute(
        &parser, response.c_str(), response.length());    
    if(error_num != 0) {
        throw LlhttpBadErrno("There is an error while executing llhttp parser.", error_num);
    }
}


/**
 * @brief Construct a new Http Request Parser object. 
 * This class parses HTTP requests.
 * 
 * @param request 
 */
HttpRequestParser::HttpRequestParser(const std::string& request) : request(request) {
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
    settings.on_message_begin = callbacks::handler_on_message_begin;
    settings.on_method = callbacks::handler_on_method;
    settings.on_url = callbacks::handler_on_uri;
    settings.on_protocol = callbacks::handler_on_protocol;
    settings.on_version = callbacks::handler_on_version;
    settings.on_header_field = callbacks::handler_on_header_field;
    settings.on_header_value = callbacks::handler_on_header_value;
    settings.on_header_value_complete = callbacks::handler_on_header_value_complete;
    settings.on_body = callbacks::handler_on_body;
}

void HttpRequestParser::parse() {
    enum llhttp_errno error_num = llhttp_execute(
        &parser, request.c_str(), request.length());    
    if(error_num != 0) {
        throw LlhttpBadErrno("There is an error while executing llhttp parser.", error_num);
    }
}

void HttpRequestParser::clear() {
    method.clear();
    uri.clear();
    protocol.clear();
    version.clear();
    headers.clear();
    body.clear();
    temporary_pair.first.clear();
    temporary_pair.second.clear();
}


// Callback methods realization.
int callbacks::handler_on_message_begin(llhttp_t* parser) {
    HttpParser* self = static_cast<HttpParser*>(parser->data);
    // Reset all the fields in the class instance.
    self->clear();

    return 0;
}

int callbacks::handler_on_uri(llhttp_t* parser, const char* at, size_t length) {
    HttpRequestParser* self = static_cast<HttpRequestParser*>(parser->data);
    self->uri.append(at, length);
    return 0;
}

int callbacks::handler_on_status(llhttp_t* parser, const char* at, size_t length) {
    HttpResponseParser* self = static_cast<HttpResponseParser*>(parser->data);
    self->status_text.append(at, length);
    return 0;
}

int callbacks::handler_on_status_complete(llhttp_t* parser) {
    HttpResponseParser* self = static_cast<HttpResponseParser*>(parser->data);
    self->status_code = parser->status_code;
    return 0;
}

int callbacks::handler_on_method(llhttp_t* parser, const char* at, size_t length) {
    HttpRequestParser* self = static_cast<HttpRequestParser*>(parser->data);
    self->method.append(at, length);
    return 0;
}

int callbacks::handler_on_protocol(llhttp_t* parser, const char* at, size_t length) {
    HttpParser* self = static_cast<HttpParser*>(parser->data);
    self->protocol.append(at, length);
    return 0;
}

int callbacks::handler_on_version(llhttp_t* parser, const char* at, size_t length) {
    HttpParser* self = static_cast<HttpParser*>(parser->data);
    self->version.append(at, length);
    return 0;
}

int callbacks::handler_on_header_field(llhttp_t* parser, const char* at, size_t length) {
    HttpParser* self = static_cast<HttpParser*>(parser->data);
    self->temporary_pair.first.append(at, length); 
    return 0;
}

int callbacks::handler_on_header_value(llhttp_t* parser, const char* at, size_t length) {
    HttpParser* self = static_cast<HttpParser*>(parser->data);
    self->temporary_pair.second.append(at, length);  
    return 0;
}

int callbacks::handler_on_header_value_complete(llhttp_t* parser) {
    HttpParser* self = static_cast<HttpParser*>(parser->data);
    self->headers.emplace(self->temporary_pair);  
    self->temporary_pair.first.clear();
    self->temporary_pair.second.clear();
    return 0;
}

int callbacks::handler_on_body(llhttp_t* parser, const char* at, size_t length) {
    HttpParser* self = static_cast<HttpParser*>(parser->data);
    self->body.append(at, length);
    return 0;
}
