#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include "json.hpp"


using uri_path_params = std::vector<std::pair<std::string, std::string>>;
using json = nlohmann::json;


struct UriParamTemplate
{
    UriParamTemplate(std::string type, std::string name)
     :
      type(type), name(name) 
        {} 
    std::string type;
    std::string name;
};


struct UriParamValue
{
    UriParamValue(std::string type, std::string value)
     :
      type(type), value(value)
        {}
    std::string type;
    std::string value;
};


struct HttpAction 
{
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


struct HttpResponse final : public HttpAction
{
    uint16_t status_code;
    std::string status_text;
};


struct HttpRequest final : public HttpAction
{
    std::string method;
    std::string uri;
    std::unordered_map<std::string, UriParamValue> path_params;
};


namespace http_headers {
    constexpr const char* content_length = "Content-Length";
    constexpr const char* content_type = "Content-Type";
    constexpr const char* date = "Date";
    constexpr const char* connection = "Connection";
    constexpr const char* cache_control = "Cache-Control";
}

namespace http_options {
    constexpr const char* protocol = "HTTP";
    constexpr const char* version_1_1 = "1.1";
}

namespace status_codes {
    constexpr std::pair<uint16_t, const char*> ok_200 = std::make_pair(200, "OK");
    constexpr std::pair<uint16_t, const char*> not_found_404 = std::make_pair(404, "NOT FOUND");
}
