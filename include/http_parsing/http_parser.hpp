#pragma once

#include <cstdint>
#include <string>
#include <unordered_map>

#include "../../third_party/llhttp/llhttp.h"

// TODO: Add getJson method for HttpParser and make it's implementation in the .cpp file.
/**
 * @brief HttpParser is a virtual class. It contains the general data and methods
 * for HttpRequestParser and HttpResponseParser classes.
 */
class HttpParser
{
    public:
        /**
         * @brief Main protocol name. (HTTP)
         */
        std::string protocol;

        /**
         * @brief HTTP uses a "<major>.<minor>" numbering scheme to indicate
         * versiongs of the protocol.
         */
        std::string version;

        /**
         * @brief Representation of the http headers with unordered-map.
         * It takes the <key>: <value> form.
         */
        std::unordered_map<std::string, std::string> headers;

        /**
         * @brief String representation of the http body. 
         */
        std::string body;

        /**
         * @brief Temporary pair presents <field: value> in the request headers.
         * The first element sets in the <handler_on_header_field>
         * The second element sets in the <handler_on_header_value>
         * The pair will be added in the <headers> in the <handler_on_header_value_complete>.
         */
        std::pair<std::string, std::string> temporary_pair;

        virtual ~HttpParser() = default;
        
        /**
         * @brief Set all the class fields to the default values.
         */
        virtual void clear() {}

    protected:
        /**
         * @brief Set the Llhttp Callback functions. 
         */
        virtual void setCallbacks() {}

        /**
         * @brief Start Llhttp parser with set callbacks. 
         */
        virtual void parse() {}
};


/**
 * @brief Child class of the HttpParser.
 * Uses to parse HTTP Responses.
 */
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


/**
 * @brief Child class of the HttpParser.
 * Uses to parse HTTP Requests.
 */
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


/**
 * @brief All the Llhttp callback functions.
 */
namespace callbacks {
    /**
        * @brief Handler start when HTTP packet begins.
        * 
        * @param parser 
        * @return int 
        */
    int handler_on_message_begin(llhttp_t* parser);

    /**
        * @brief Handler start when Llhttp finds http body.
        * 
        * @param parser 
        * @return int 
        */
    int handler_on_body(llhttp_t* parser, const char* at, size_t length);

    /**
    * @brief Handler starts when llhttp finds HTTP method:
    * [GET, POST, PUT, DELETE, PATCH, HEAD, OPTIONS, TRACE, CONNECT]
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
