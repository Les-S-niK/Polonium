
#include "polonium/http/request_parser.hpp"

#include <llhttp.h>

#include <format>

#include "polonium/http/http.hpp"
#include "polonium/polonium_logger.hpp"

HttpRequestParser::HttpRequestParser()
    : logger_(PoloniumLogger::getInstance()) {
    logger_->trace(__func__);
    logger_->debug("Llhttp Parser initialization.");
    request_ = HttpRequest();
    llhttp_settings_init(&settings_);
    llhttp_init(&parser_, HTTP_REQUEST, &settings_);
    parser_.data = this;
    setCallbacks();
}

auto HttpRequestParser::setCallbacks() -> void {
    logger_->trace(__func__);
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

auto HttpRequestParser::reset() -> void {
    logger_->trace(__func__);
    request_ = HttpRequest{};
    temporary_pair_.first.clear();
    temporary_pair_.second.clear();
    is_complete_ = false;
    is_keep_alive_ = false;
    parsed_bytes_ = 0;
}

auto HttpRequestParser::feed(std::string_view to_accumulate)
    -> HttpRequestParserStatus {
    logger_->trace(__func__);
    parsed_bytes_ = raw_request_.size() - parsed_bytes_;
    if (!to_accumulate.empty()) {
        logger_->debug(
            std::format("Insert <to_accumulate> buffer.\nRaw request size: "
                        "{}\nTo accumulate size: {}",
                        raw_request_.size(), to_accumulate.size()));
        raw_request_.insert(raw_request_.end(), to_accumulate.begin(),
                            to_accumulate.end());
    }
    if (hasRemainingData()) {
        removeParsed();
    }
    return parseAccumulated();
}

auto HttpRequestParser::parseAccumulated() -> HttpRequestParserStatus {
    logger_->trace(__func__);
    if (is_complete_) {
        return HttpRequestParserStatus::Complete;
    }

    logger_->debug(std::format("Executing llhttp parser. Raw request size: {}",
                               raw_request_.size()));
    llhttp_errno_t parser_errno =
        llhttp_execute(&parser_, raw_request_.data(), raw_request_.size());

    if (parser_errno != HPE_OK) {
        logger_->error(std::format("Llhttp Parser returned bad HPE. HPE: {}",
                                   llhttp_errno_name(parser_errno)));
        return HttpRequestParserStatus::Error;
    }

    if (is_complete_) {
        logger_->debug("Llhttp parsing completed.");
        return HttpRequestParserStatus::Complete;
    }
    logger_->debug("Llhttp parser waits for the next message.");
    return HttpRequestParserStatus::NeedMore;
}
