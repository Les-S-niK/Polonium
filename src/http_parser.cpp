
#include <iostream>

#include "../include/http_parser.hpp"


HttpRequestsParser::HttpRequestsParser(const std::string& request) : request(request) {
    llhttp_settings_init(&settings);
    setCallbacks();
    llhttp_init(&parser, HTTP_REQUEST, &settings);
    parse();
}

void HttpRequestsParser::setCallbacks() {
    settings.on_message_begin = handler_on_message_begin;
}

void HttpRequestsParser::parse() {
    enum llhttp_errno error_num = llhttp_execute(&parser, request.c_str(), request.length());    
    if(error_num != 0) {
        // TODO: Throw an exception class here in future.
        throw "The error code is bad.";
    }
}

// Callback methods realization.
int HttpRequestsParser::handler_on_message_begin(llhttp_t* parser) {
    std::cout << "Begin" << std::endl;
    return 0;
}
