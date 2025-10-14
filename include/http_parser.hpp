#pragma once

#include <string>
#include <unordered_map>

#include "../third_party/llhttp/llhttp.h"


class HttpRequestsParser
{
    public:
        std::string url;
        std::string method;
        std::string protocol;
        std::string version;
        std::unordered_map<std::string, std::string> headers;
        std::string body;

        HttpRequestsParser(const std::string& request);
        
    private:
        llhttp_t parser;
        llhttp_settings_t settings;
        const std::string request;
        /**
         * @brief Temporary pair presents <field: value> in the request headers.
         * The first element sets in the <handler_on_header_field>
         * The second element sets in the <handler_on_header_value>
         * The pair will be added in the <headers> in the <handler_on_header_value_complete>.
         */
        std::pair<std::string, std::string> temporary_pair;

        void setCallbacks();
        void parse();

        // Callback llhttp methods.

        /**
         * @brief Handler start when http packet begins.
         * 
         * @param parser 
         * @return int 
         */
        static int handler_on_message_begin(llhttp_t* parser);

        /**
         * @brief Handler start when llhttp finds http body.
         * 
         * @param parser 
         * @return int 
         */
        static int handler_on_body(llhttp_t* parser, const char* at, size_t length);
        
        /**
        * @brief Handler starts when llhttp finds url.
        * 
        * @param parser 
        * @param at
        * @param length 
        * @return int - erorr code.
        */
        static int handler_on_url(llhttp_t* parser, const char* at, size_t length);

        /**
        * @brief Handler starts when llhttp finds http method [GET, POST, PUT, DELETE, PATCH, HEAD, OPTIONS, TRACE, CONNECT]
        * 
        * @param parser 
        * @param at 
        * @param length 
        * @return int 
        */
        static int handler_on_method(llhttp_t* parser, const char* at, size_t length);

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
        static int handler_on_version(llhttp_t* parser, const char* at, size_t length);

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
};
