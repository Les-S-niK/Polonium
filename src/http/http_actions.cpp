
#include "polonium/http/http_actions.hpp"

#include <format>

[[nodiscard]] auto polonium::HttpAction::getJson() const -> json {
    return json_actions::parseStringJson(body);
}

polonium::HttpResponse::HttpResponse(
    const std::string& protocol, const std::string& version,
    const std::pair<const uint16_t, const char*> status)
    : status_code(status.first), status_text(status.second) {
    this->protocol = protocol;
    this->version = version;
}
polonium::HttpResponse::HttpResponse(const ApiResponse& api_response)
    : HttpResponse(http_options::protocol, http_options::version_1_1,
                   api_response.getStatusCode()) {
    headers[http_headers::content_length] =
        std::to_string(api_response.getContent().size());
    headers[http_headers::content_type] = "application/json; charset=utf-8";

    for (std::pair<std::string, std::string> header :
         api_response.getHeaders()) {
        headers.insert(std::move(header));
    }
    body = api_response.getContent();
}

auto polonium::response_templates::create404ErrorResponse() -> HttpResponse {
    std::string json_str =
        std::format(R"({{"status": "{}", "message": "{}"}})",
                    status_codes::client_errors_4xx::not_found_404.first,
                    status_codes::client_errors_4xx::not_found_404.second);
    HttpResponse error_404_response(
        http_options::protocol, http_options::version_1_1,
        status_codes::client_errors_4xx::not_found_404);
    error_404_response.headers[http_headers::content_length] =
        std::to_string(json_str.size());
    error_404_response.headers[http_headers::content_type] =
        "application/json; charset=utf-8";
    error_404_response.body = json_str;

    return error_404_response;
}

auto polonium::response_templates::get404ErrorResponse() -> HttpResponse {
    static HttpResponse error_404_response =
        response_templates::create404ErrorResponse();
    return error_404_response;
}
