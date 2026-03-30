
#include "polonium/http/http.hpp"

auto response_templates::create404ErrorResponse() -> HttpResponse {
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


