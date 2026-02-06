#pragma once

#include <cstddef>
#include <string>

#include <llhttp.h>
#include "polonium/http/http.hpp"
#include "polonium/polonium_logger.hpp"


enum class HttpRequestParserStatus
{
    Complete = 0,
    NeedMore = 1,
    Error = 2
};

/**
 * @brief This parser class uses to parse raw HTTP requests
 * and serialize it into HttpRequest object.
 */
class HttpRequestParser
{
    public:
        HttpRequestParser(PoloniumLogger& logger);
        HttpRequestParser(const HttpRequestParser&) = delete;
        HttpRequestParser(HttpRequestParser&&) = delete;
        HttpRequestParser operator=(const HttpRequestParser&) = delete;
        HttpRequestParser operator=(HttpRequestParser&&) = delete;
        ~HttpRequestParser() {
            logger_.trace(__func__);
            llhttp_finish(&parser_);
        }

        void reset();
        HttpRequestParserStatus feed(std::string_view to_accumulate);
        
        inline const HttpRequest& getRequest() const {
            logger_.trace(__func__);
            return request_;
        }
        inline bool hasRemainingData() const {
            logger_.trace(__func__);
            return parsed_bytes_ < raw_request_.size();
        }
        inline bool isComplete() const {
            logger_.trace(__func__);
            return is_complete_;
        }
        inline bool isKeepAlive() const {
            logger_.trace(__func__);
            return is_keep_alive_;
        }
        inline void removeParsed() {
            logger_.trace(__func__);
            raw_request_.erase(raw_request_.begin(), raw_request_.begin() + parsed_bytes_);
        }
    
    private:
        llhttp_t parser_;
        llhttp_settings_t settings_;
        HttpRequest request_;
        std::string raw_request_;
        bool is_complete_ = 0;
        bool is_keep_alive_ = 0;
        size_t parsed_bytes_ = 0;
        PoloniumLogger& logger_;
        /**
         * @brief Temporary pair represents <field: value> in the request headers.
         * The first element sets in the <handler_on_header_field>
         * The second element sets in the <handler_on_header_value>
         * The pair will be added in the <headers> in the <handler_on_header_value_complete>.
         */
        std::pair<std::string, std::string> temporary_pair_;

        HttpRequestParserStatus parseAccumulated();
        void setCallbacks();

        static inline int handler_on_message_begin(llhttp_t* parser) {
            HttpRequestParser* self = static_cast<HttpRequestParser*>(parser->data);
            // Reset all the fields in the class instance.
            self->reset();
            return 0;
        }

        /**
        * @brief Handler calls when llhttp finds HTTP method:
        * [GET, POST, PUT, DELETE, PATCH, HEAD, OPTIONS, TRACE, CONNECT]
        */
        static inline int handler_on_method(llhttp_t* parser, const char* at, size_t length) {
            HttpRequestParser* self = static_cast<HttpRequestParser*>(parser->data);
            self->request_.method.assign(at, length);
            return 0;
        }

        static inline int handler_on_uri(llhttp_t* parser, const char* at, size_t length) {
            HttpRequestParser* self = static_cast<HttpRequestParser*>(parser->data);
            self->request_.uri.assign(at, length);
            return 0;
        }
        
        /**
        * @brief Handler calls when llhttp finds protocol type [HTTP for example]
        */
        static inline int handler_on_protocol(llhttp_t* parser, const char* at, size_t length) {
            HttpRequestParser* self = static_cast<HttpRequestParser*>(parser->data);
            self->request_.protocol.assign(at, length);
            return 0;
        }

        /**
        * @brief Handler calls when llhttp finds protocol version. [1.1; 2; etc... for example]
        */
        static inline int handler_on_protocol_version(llhttp_t* parser, const char* at, size_t length) {
            HttpRequestParser* self = static_cast<HttpRequestParser*>(parser->data);
            self->request_.version.assign(at, length);
            self->is_keep_alive_ = llhttp_should_keep_alive(parser);
            return 0;
        }
        
        /**
        * @brief Handler calls when llhttp finds header field [<field>: value];
        */
        static inline int handler_on_header_field(llhttp_t* parser, const char* at, size_t length) {
            HttpRequestParser* self = static_cast<HttpRequestParser*>(parser->data);
            self->temporary_pair_.first.assign(at, length); 
            return 0;
        }
        
        static inline int handler_on_header_value(llhttp_t* parser, const char* at, size_t length) {
            HttpRequestParser* self = static_cast<HttpRequestParser*>(parser->data);
            self->temporary_pair_.second.assign(at, length);  
            return 0;
        }
        
        static inline int handler_on_header_value_complete(llhttp_t* parser) {
            HttpRequestParser* self = static_cast<HttpRequestParser*>(parser->data);
            self->request_.headers.emplace(std::move(self->temporary_pair_));  
            self->temporary_pair_ = {};
            return 0;
        }
        
        static inline int handler_on_body(llhttp_t* parser, const char* at, size_t length) {
            HttpRequestParser* self = static_cast<HttpRequestParser*>(parser->data);
            self->request_.body.append(at, length);
            return 0;
        }
        
        static inline int handler_on_message_complete(llhttp_t* parser) {
            HttpRequestParser* self = static_cast<HttpRequestParser*>(parser->data);
            self->is_complete_ = true;
            return 0;
        }
};
