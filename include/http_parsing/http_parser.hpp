#pragma once

#include <cstddef>
#include <string>
#include <vector>

#include "llhttp.h"
#include "http.hpp"


/**
 * @brief HttpParser is a virtual class. It contains the general data and methods
 * for HttpRequestParser and HttpResponseParser classes.
 */
class HttpParser
{
    public:
        bool is_complete;
        bool is_keep_alive;
        size_t parsed_bytes = 0;

        virtual ~HttpParser() = default;

        /**
         * @brief Set all the class fields to the default values.
         */
        virtual void clear() = 0;

        virtual void feedData(const std::vector<std::byte>& to_accumulate) = 0;

        bool isComplete() const noexcept { return is_complete; }

    protected:  
        HttpAction* http_parsed;
        
        /**
         * @brief Temporary pair presents <field: value> in the request headers.
         * The first element sets in the <handler_on_header_field>
         * The second element sets in the <handler_on_header_value>
         * The pair will be added in the <headers> in the <handler_on_header_value_complete>.
         */
        std::pair<std::string, std::string> temporary_pair;

        /**
         * @brief Set the Llhttp Callback functions. 
         */
        virtual void setCallbacks() = 0;

        /**
         * @brief Initial parser method. Do initial stuff like set llhttp settings and etc.
         */
        virtual void initParser() = 0;

        /**
         * @brief Parse all the accumulated HTTP packet.
         */
        virtual void parseAccumulated() = 0;

        /**
         * @brief Handler calls when HTTP packet begins.
         * 
         * @param parser 
         * @return int 
         */
        static int handler_on_message_begin(llhttp_t* parser);

        /**
         * @brief Handler calls when llhttp finds protocol type [HTTP for example]
         * 
         * @param parser 
         * @param at 
         * @param length 
         * @return int 
         */
        static int handler_on_protocol(llhttp_t* parser, const char* at, size_t length);

        /**
         * @brief Handler calls when llhttp finds protocol version. [1.1; 2; etc... for example]
         * 
         * @param parser 
         * @param at 
         * @param length 
         * @return int 
         */
        static int handler_on_protocol_version(llhttp_t* parser, const char* at, size_t length);

        /**
         * @brief Handler calls when llhttp finds header field [<field>: value];
         * 
         * @param parser 
         * @param at 
         * @param length 
         * @return int 
         */
        static int handler_on_header_field(llhttp_t* parser, const char* at, size_t length);

        /**
         * @brief Handler calls when llhttp finds header value [field: <value>];
         * 
         * @param parser 
         * @param at 
         * @param length 
         * @return int 
         */
        static int handler_on_header_value(llhttp_t* parser, const char* at, size_t length);

        /**
         * @brief Handler calls when llhttp ends the headers parsing.
         * 
         * @param parser 
         * @return int 
         */
        static int handler_on_header_value_complete(llhttp_t* parser);

        /**
         * @brief Handler calls when Llhttp finds http body.
         * 
         * @param parser 
         * @return int 
         */
        static int handler_on_body(llhttp_t* parser, const char* at, size_t length);

        /**
         * @brief Handler calls when Llhttp ends the parsing.
         * 
         * @param parser 
         * @return int 
         */
        static int handler_on_message_complete(llhttp_t* parser);
};


/**
 * @brief Child class of the HttpParser.
 * Uses to parse HTTP Responses.
 */
class HttpResponseParser : public HttpParser
{
    public:
        HttpResponseParser();
        HttpResponseParser(const HttpResponseParser&) = delete;
        HttpResponseParser(HttpResponseParser&&) = delete;
        HttpResponseParser operator=(const HttpResponseParser&) = delete;
        HttpResponseParser operator=(HttpResponseParser&&) = delete;
        ~HttpResponseParser();

        void clear() override final;
        void feedData(const std::vector<std::byte>& to_accumulate) override final;

        HttpResponse getParsed();
        
    private:
        HttpResponse response;
        llhttp_t parser;
        llhttp_settings_t settings;
        std::vector<std::byte> raw_response;

        void initParser() override final;
        void setCallbacks() override final;
        void parseAccumulated() override final;

        /**
         * @brief Handler calls when llhttp finds Response status text;
         * 
         * @param parser 
         * @param at 
         * @param length 
         * @return int 
         */
        static int handler_on_status(llhttp_t* parser, const char* at, size_t length);

        /**
         * @brief Handler calls when llhttp completes the parsing of Response status text;
         * 
         * @param parser 
         * @return int 
         */
        static int handler_on_status_complete(llhttp_t* parser);
};


/**
 * @brief Child class of the HttpParser.
 * Uses to parse HTTP Requests.
 */
class HttpRequestParser : public HttpParser
{
    public:
        HttpRequestParser();
        HttpRequestParser(const HttpRequestParser&) = delete;
        HttpRequestParser(HttpRequestParser&&) = delete;
        HttpRequestParser operator=(const HttpRequestParser&) = delete;
        HttpRequestParser operator=(HttpRequestParser&&) = delete;
        ~HttpRequestParser();

        void clear() override final;
        void feedData(const std::vector<std::byte>& to_accumulate) override final;
        HttpRequest getParsed();
    
    private:
        HttpRequest request;
        llhttp_t parser;
        llhttp_settings_t settings;
        std::vector<std::byte> raw_request;

        void initParser() override final;
        void setCallbacks() override final;
        void parseAccumulated() override final;

        /**
         * @brief Handler calls when llhttp finds HTTP method:
         * [GET, POST, PUT, DELETE, PATCH, HEAD, OPTIONS, TRACE, CONNECT]
         * 
         * @param parser 
         * @param at 
         * @param length 
         * @return int 
         */
        static int handler_on_method(llhttp_t* parser, const char* at, size_t length);

        /**
         * @brief Handler calls when llhttp finds uri.
         * 
         * @param parser 
         * @param at
         * @param length 
         * @return int - erorr code.
         */
        static int handler_on_uri(llhttp_t* parser, const char* at, size_t length);
};
