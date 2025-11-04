
#include "http_parsing/request_parser.hpp"
#include "http_parsing/request_exceptions.hpp"
#include "http.hpp"
#include "llhttp.h"


HttpRequestParser::HttpRequestParser() {
    request_ = HttpRequest();
    llhttp_settings_init(&settings_);
    llhttp_init(&parser_, HTTP_REQUEST, &settings_);
    parser_.data = this;
    setCallbacks();
}

void HttpRequestParser::setCallbacks() {
    settings_.on_message_begin = handler_on_message_begin;
    settings_.on_method = handler_on_method;
    settings_.on_url = handler_on_uri;
    settings_.on_protocol = handler_on_protocol;
    settings_.on_version = handler_on_protocol_version;
    settings_.on_header_field = handler_on_header_field;
    settings_.on_header_value = handler_on_header_value;
    settings_.on_header_value_complete = handler_on_header_value_complete;
    settings_.on_body = handler_on_body;
    settings_.on_message_complete = handler_on_message_complete;
}

void HttpRequestParser::reset() {
    request_ = HttpRequest{};
    temporary_pair_.first.clear();
    temporary_pair_.second.clear();
    is_complete_ = false;
    is_keep_alive_ = false;
    parsed_bytes_ = 0;
}

HttpRequestParserStatus HttpRequestParser::feed(const std::string& to_accumulate) {
    parsed_bytes_ = raw_request_.size() - parsed_bytes_;
    if(to_accumulate.size() > 0) {
        raw_request_.insert(
            raw_request_.end(),
            to_accumulate.begin(),
            to_accumulate.end()
        );
    }
    if(hasRemainingData()) removeParsed();
    return parseAccumulated();
}

HttpRequestParserStatus HttpRequestParser::parseAccumulated() {
    if(is_complete_) return HttpRequestParserStatus::Complete;

    llhttp_errno_t parser_errno = llhttp_execute(
        &parser_,
        raw_request_.data(),
        raw_request_.size()
    );

    if(parser_errno != HPE_OK) {
        throw llhttp_bad_errno("Can't correctly execute llhttp parser.", parser_errno);
    }

    if(is_complete_) {
        return HttpRequestParserStatus::Complete;
    }
    return HttpRequestParserStatus::NeedMore;    
}
