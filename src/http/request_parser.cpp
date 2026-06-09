#include "polonium/http/request_parser.hpp"

#include <llhttp.h>

#include <format>

#include "polonium/app/polonium_logger.hpp"

polonium::HttpRequestParser::~HttpRequestParser() {
    logger_->trace(__func__);
    llhttp_finish(&parser_);
}

auto polonium::HttpRequestParser::getRequest() const -> const HttpRequest& {
    logger_->trace(__func__);
    return request_;
}

auto polonium::HttpRequestParser::isComplete() const -> bool {
    logger_->trace(__func__);
    return is_complete_;
}

auto polonium::HttpRequestParser::isKeepAlive() const -> bool {
    logger_->trace(__func__);
    return is_keep_alive_;
}

auto polonium::HttpRequestParser::handlerOnMessageBegin(llhttp_t* parser)
    -> int {
    auto* self = static_cast<HttpRequestParser*>(parser->data);
    self->reset();
    return 0;
}

auto polonium::HttpRequestParser::handlerOnMethod(llhttp_t* parser,
                                                  const char* chunk_ptr,
                                                  size_t length) -> int {
    auto* self = static_cast<HttpRequestParser*>(parser->data);
    self->request_.method.assign(chunk_ptr, length);
    return 0;
}

auto polonium::HttpRequestParser::handlerOnUri(llhttp_t* parser,
                                               const char* chunk_ptr,
                                               size_t length) -> int {
    auto* self = static_cast<HttpRequestParser*>(parser->data);
    self->request_.uri.assign(chunk_ptr, length);
    return 0;
}

auto polonium::HttpRequestParser::handlerOnProtocol(llhttp_t* parser,
                                                    const char* chunk_ptr,
                                                    size_t length) -> int {
    auto* self = static_cast<HttpRequestParser*>(parser->data);
    self->request_.protocol.assign(chunk_ptr, length);
    return 0;
}

auto polonium::HttpRequestParser::handlerOnProtocolVersion(
    llhttp_t* parser, const char* chunk_ptr, size_t length) -> int {
    auto* self = static_cast<HttpRequestParser*>(parser->data);
    self->request_.version.assign(chunk_ptr, length);
    self->is_keep_alive_ = (llhttp_should_keep_alive(parser) != 0);
    return 0;
}

auto polonium::HttpRequestParser::handlerOnHeaderField(llhttp_t* parser,
                                                       const char* chunk_ptr,
                                                       size_t length) -> int {
    auto* self = static_cast<HttpRequestParser*>(parser->data);
    self->temporary_pair_.first.assign(chunk_ptr, length);
    return 0;
}

auto polonium::HttpRequestParser::handlerOnHeaderValue(llhttp_t* parser,
                                                       const char* chunk_ptr,
                                                       size_t length) -> int {
    auto* self = static_cast<HttpRequestParser*>(parser->data);
    self->temporary_pair_.second.assign(chunk_ptr, length);
    return 0;
}

auto polonium::HttpRequestParser::handlerOnHeaderValueComplete(llhttp_t* parser)
    -> int {
    auto* self = static_cast<HttpRequestParser*>(parser->data);
    self->request_.headers.emplace(std::move(self->temporary_pair_));
    self->temporary_pair_ = {};
    return 0;
}

auto polonium::HttpRequestParser::handlerOnBody(llhttp_t* parser,
                                                const char* chunk_ptr,
                                                size_t length) -> int {
    auto* self = static_cast<HttpRequestParser*>(parser->data);
    self->request_.body.append(chunk_ptr, length);
    return 0;
}

auto polonium::HttpRequestParser::handlerOnMessageComplete(llhttp_t* parser)
    -> int {
    auto* self = static_cast<HttpRequestParser*>(parser->data);
    self->is_complete_ = true;
    return 0;
}

polonium::HttpRequestParser::HttpRequestParser()
    : logger_(polonium::PoloniumLogger::getInstance()) {
    logger_->trace(__func__);
    logger_->debug("Llhttp Parser initialization.");
    request_ = HttpRequest();
    llhttp_settings_init(&settings_);
    llhttp_init(&parser_, HTTP_REQUEST, &settings_);
    parser_.data = this;
    setCallbacks();
}

auto polonium::HttpRequestParser::setCallbacks() -> void {
    logger_->trace(__func__);
    settings_.on_message_begin = handlerOnMessageBegin;
    settings_.on_method = handlerOnMethod;
    settings_.on_url = handlerOnUri;
    settings_.on_protocol = handlerOnProtocol;
    settings_.on_version = handlerOnProtocolVersion;
    settings_.on_header_field = handlerOnHeaderField;
    settings_.on_header_value = handlerOnHeaderValue;
    settings_.on_header_value_complete = handlerOnHeaderValueComplete;
    settings_.on_body = handlerOnBody;
    settings_.on_message_complete = handlerOnMessageComplete;
}

auto polonium::HttpRequestParser::reset() -> void {
    logger_->trace(__func__);
    request_ = HttpRequest{};
    temporary_pair_.first.clear();
    temporary_pair_.second.clear();
    is_complete_ = false;
    is_keep_alive_ = false;
}

auto polonium::HttpRequestParser::feed(std::string_view to_accumulate)
    -> HttpRequestParserStatus {
    logger_->trace(__func__);
    if (!to_accumulate.empty()) {
        logger_->debug(
            std::format("Parse accumulated.\n"
                        "To accumulate size: {}",
                        to_accumulate.size()));
        return parseAccumulated(to_accumulate);
    }
    return is_complete_ ? HttpRequestParserStatus::Complete
                        : HttpRequestParserStatus::NeedMore;
}

auto polonium::HttpRequestParser::parseAccumulated(std::string_view data)
    -> HttpRequestParserStatus {
    logger_->trace(__func__);
    if (is_complete_) {
        return HttpRequestParserStatus::Complete;
    }

    logger_->debug(std::format("Executing llhttp parser on chunk of size: {}",
                               data.size()));
    llhttp_errno_t parser_errno =
        llhttp_execute(&parser_, data.data(), data.size());

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
