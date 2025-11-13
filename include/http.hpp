#pragma once

#include <string>
#include <unordered_map>

#include "json.hpp"


using json = nlohmann::json;


class HttpAction 
{
    public:
        virtual ~HttpAction() = default;

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
         * @brief Get the Json object from the HTTP body.
         * 
         * @return json (nlohmann::json). Return an empty json if parser can't parse the body.
         */
        inline json getJson() {
            if (!json::accept(body)) {
                /* Return an empty json if body can't be parsed. */
                return json{};
            }
            return json::parse(body);
        }
};


class HttpResponse : public HttpAction
{
    public:
        uint16_t status_code;
        std::string status_text;
};


class HttpRequest : public HttpAction
{
    public:
        std::string method;
        std::string uri;
};
