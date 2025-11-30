#pragma once

#include <cstdint>
#include <string>
#include <unordered_map>
#include <utility>

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


namespace http_headers {
    constexpr const char* content_length = "Content-Length";
    constexpr const char* content_type = "Content-Type";
    constexpr const char* date = "Date";
}

namespace http_options {
    constexpr const char* protocol = "HTTP";
    constexpr const char* version_1_1 = "1.1";
}

namespace status_codes {
    constexpr std::pair<uint16_t, const char*> ok_200 = std::make_pair(200, "OK");
    constexpr std::pair<uint16_t, const char*> not_found_404 = std::make_pair(404, "NOT FOUND");
}
