
#pragma once

#include <string>
#include <unordered_map>

#include "polonium/json_parser.hpp"
#include "polonium/routing/api_responses.hpp"
#include "polonium/routing/uri_params.hpp"

namespace polonium {
struct HttpAction {
    HttpAction() = default;
    HttpAction(const HttpAction&) = default;
    HttpAction(HttpAction&&) = default;
    auto operator=(const HttpAction&) -> HttpAction& = default;
    auto operator=(HttpAction&&) -> HttpAction& = default;
    virtual ~HttpAction() = default;

    /**
     * @brief Get the Json object from the HTTP body.
     *
     * @return json (std::unordered_map<std::string, std::string>). Returns an
     * empty json if parser can't parse the body.
     */
    [[nodiscard]] auto getJson() const -> json;

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
};

struct HttpResponse final : public HttpAction {
    HttpResponse() = default;
    HttpResponse(const std::string& protocol, const std::string& version,
                 std::pair<const uint16_t, const char*> status);
    explicit HttpResponse(const ApiResponse& api_response);
    uint16_t status_code{};
    std::string status_text;
};

struct HttpRequest final : public HttpAction {
    std::string method;
    std::string uri;
    std::unordered_map<std::string, UriParamValue> path_params;
};

namespace response_templates {
auto create404ErrorResponse() -> HttpResponse;
auto get404ErrorResponse() -> HttpResponse;
}  // namespace response_templates
}  // namespace polonium
