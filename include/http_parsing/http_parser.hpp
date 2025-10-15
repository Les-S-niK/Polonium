#pragma once

#include <string>
#include <unordered_map>

#include "../third_party/llhttp/llhttp.h"

// TODO: Add getJson method for HttpParser and make it's implementation in the .cpp file.
/**
 * @brief HttpParser is a virtual class. It contains the general data and methods
 * for HttpRequestParser and HttpResponseParser classes.
 * 
 */
class HttpParser
{
    public:
        std::string protocol;
        std::string version;
        std::unordered_map<std::string, std::string> headers;
        std::string body;
        /**
         * @brief Temporary pair presents <field: value> in the request headers.
         * The first element sets in the <handler_on_header_field>
         * The second element sets in the <handler_on_header_value>
         * The pair will be added in the <headers> in the <handler_on_header_value_complete>.
         */
        std::pair<std::string, std::string> temporary_pair;
 
        virtual ~HttpParser() = default;
        virtual void clear() {}

    protected:
        virtual void setCallbacks() {}
        virtual void parse() {}
};


class HttpResponseParser : public HttpParser
{
    public:
        uint16_t status_code;
        std::string status_text;

        HttpResponseParser(const std::string& response);
        ~HttpResponseParser();
        void clear() override final;
        
    private:
        llhttp_t parser;
        llhttp_settings_t settings;
        const std::string response;

        void setCallbacks() override final;
        void parse() override final;
};


class HttpRequestParser : public HttpParser
{
    public:
        std::string method;
        std::string uri;

        HttpRequestParser(const std::string& request);
        ~HttpRequestParser();
        void clear() override final;
        
    private:
        llhttp_t parser;
        llhttp_settings_t settings;
        const std::string request;

        void setCallbacks() override final;
        void parse() override final;
        
};


namespace callbacks {
    // Callback llhttp methods.
    /**
        * @brief Handler start when http packet begins.
        * 
        * @param parser 
        * @return int 
        */
    int handler_on_message_begin(llhttp_t* parser);

    /**
        * @brief Handler start when llhttp finds http body.
        * 
        * @param parser 
        * @return int 
        */
    int handler_on_body(llhttp_t* parser, const char* at, size_t length);

    /**
    * @brief Handler starts when llhttp finds http method [GET, POST, PUT, DELETE, PATCH, HEAD, OPTIONS, TRACE, CONNECT]
    * 
    * @param parser 
    * @param at 
    * @param length 
    * @return int 
    */
    int handler_on_method(llhttp_t* parser, const char* at, size_t length);

    /**
    * @brief Handler starts when llhttp finds uri.
    * 
    * @param parser 
    * @param at
    * @param length 
    * @return int - erorr code.
    */
    int handler_on_uri(llhttp_t* parser, const char* at, size_t length);

    /**
    * @brief Handler starts when llhttp finds Response status text;
    * 
    * @param parser 
    * @param at 
    * @param length 
    * @return int 
    */
    int handler_on_status(llhttp_t* parser, const char* at, size_t length);

    /**
    * @brief Handler starts when llhttp completes the parsing of Response status text;
    * 
    * @param parser 
    * @return int 
    */
    int handler_on_status_complete(llhttp_t* parser);
    
    /**
    * @brief Handler starts when llhttp finds protocol type [HTTP for example]
    * 
    * @param parser 
    * @param at 
    * @param length 
    * @return int 
    */
    int handler_on_protocol(llhttp_t* parser, const char* at, size_t length);

    /**
    * @brief Handler starts when llhttp finds protocol version. [1.1; 2; etc... for example]
    * 
    * @param parser 
    * @param at 
    * @param length 
    * @return int 
    */
    int handler_on_version(llhttp_t* parser, const char* at, size_t length);

    /**
    * @brief Handler starts when llhttp finds header field [<field>: value];
    * 
    * @param parser 
    * @param at 
    * @param length 
    * @return int 
    */
    int handler_on_header_field(llhttp_t* parser, const char* at, size_t length);

    /**
    * @brief Handler starts when llhttp finds header value [field: <value>];
    * 
    * @param parser 
    * @param at 
    * @param length 
    * @return int 
    */
    int handler_on_header_value(llhttp_t* parser, const char* at, size_t length);

    /**
        * @brief Handler starts when llhttp ends the headers parsing.
        * 
        * @param parser 
        * @return int 
        */
    int handler_on_header_value_complete(llhttp_t* parser);
}
