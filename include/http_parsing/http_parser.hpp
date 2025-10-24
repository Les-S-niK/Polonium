#pragma once

#include <string>
#include <vector>

#include "../../third_party/llhttp/llhttp.h"
#include "./http.hpp"


/**
 * @brief HttpParser is a virtual class. It contains the general data and methods
 * for HttpRequestParser and HttpResponseParser classes.
 */
class HttpParser
{
    public:
        virtual ~HttpParser() = default;

        /**
         * @brief Set all the class fields to the default values.
         */
        virtual void clear() = 0;

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
         * @brief Start Llhttp parser with set callbacks. 
         */
        virtual void parse() = 0;

        /**
         * @brief Handler start when HTTP packet begins.
         * 
         * @param parser 
         * @return int 
         */
        static int handler_on_message_begin(llhttp_t* parser);

        /**
         * @brief Handler starts when llhttp finds protocol type [HTTP for example]
         * 
         * @param parser 
         * @param at 
         * @param length 
         * @return int 
         */
        static int handler_on_protocol(llhttp_t* parser, const char* at, size_t length);

        /**
         * @brief Handler starts when llhttp finds protocol version. [1.1; 2; etc... for example]
         * 
         * @param parser 
         * @param at 
         * @param length 
         * @return int 
         */
        static int handler_on_protocol_version(llhttp_t* parser, const char* at, size_t length);

        /**
         * @brief Handler starts when llhttp finds header field [<field>: value];
         * 
         * @param parser 
         * @param at 
         * @param length 
         * @return int 
         */
        static int handler_on_header_field(llhttp_t* parser, const char* at, size_t length);

        /**
         * @brief Handler starts when llhttp finds header value [field: <value>];
         * 
         * @param parser 
         * @param at 
         * @param length 
         * @return int 
         */
        static int handler_on_header_value(llhttp_t* parser, const char* at, size_t length);

        /**
         * @brief Handler starts when llhttp ends the headers parsing.
         * 
         * @param parser 
         * @return int 
         */
        static int handler_on_header_value_complete(llhttp_t* parser);

        /**
         * @brief Handler start when Llhttp finds http body.
         * 
         * @param parser 
         * @return int 
         */
        static int handler_on_body(llhttp_t* parser, const char* at, size_t length);
};


/**
 * @brief Child class of the HttpParser.
 * Uses to parse HTTP Responses.
 */
class HttpResponseParser : public HttpParser
{
    public:
        HttpResponse& response;
        HttpResponseParser(const std::vector<std::byte>& raw_response, HttpResponse& response);
        HttpResponseParser(const HttpResponseParser&) = delete;
        HttpResponseParser(HttpResponseParser&&) = delete;
        HttpResponseParser operator=(const HttpResponseParser&) = delete;
        HttpResponseParser operator=(HttpResponseParser&&) = delete;
        ~HttpResponseParser();

        void clear() override final;
        
    private:
        llhttp_t parser;
        llhttp_settings_t settings;
        const std::vector<std::byte>& raw_response;

        void setCallbacks() override final;
        void parse() override final;

        /**
         * @brief Handler starts when llhttp finds Response status text;
         * 
         * @param parser 
         * @param at 
         * @param length 
         * @return int 
         */
        static int handler_on_status(llhttp_t* parser, const char* at, size_t length);

        /**
         * @brief Handler starts when llhttp completes the parsing of Response status text;
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
        HttpRequest& request;
        HttpRequestParser(const std::vector<std::byte>& raw_request, HttpRequest& request);
        HttpRequestParser(const HttpRequestParser&) = delete;
        HttpRequestParser(HttpRequestParser&&) = delete;
        HttpRequestParser operator=(const HttpRequestParser&) = delete;
        HttpRequestParser operator=(HttpRequestParser&&) = delete;
        ~HttpRequestParser();

        void clear() override final;
    
    private:
        llhttp_t parser;
        llhttp_settings_t settings;
        const std::vector<std::byte>& raw_request;

        void setCallbacks() override final;
        void parse() override final;

        /**
         * @brief Handler starts when llhttp finds HTTP method:
         * [GET, POST, PUT, DELETE, PATCH, HEAD, OPTIONS, TRACE, CONNECT]
         * 
         * @param parser 
         * @param at 
         * @param length 
         * @return int 
         */
        static int handler_on_method(llhttp_t* parser, const char* at, size_t length);

        /**
         * @brief Handler starts when llhttp finds uri.
         * 
         * @param parser 
         * @param at
         * @param length 
         * @return int - erorr code.
         */
        static int handler_on_uri(llhttp_t* parser, const char* at, size_t length);
};
