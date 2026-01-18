
#pragma once

#include <string>
#include <unordered_map>
#include <format>

#include "json_parser.hpp"
#include "routing/uri_params.hpp"


namespace http_headers {
    constexpr const char* cache_control = "Cache-Control";
    constexpr const char* connection = "Connection";
    constexpr const char* date = "Date";
    constexpr const char* pragma = "Pragma";
    constexpr const char* trailer = "Trailer";
    constexpr const char* transfer_encoding = "Transfer-Encoding";
    constexpr const char* upgrade = "Upgrade";
    constexpr const char* via = "Via";
    constexpr const char* warning = "Warning";
    
    constexpr const char* accept = "Accept";
    constexpr const char* accept_charset = "Accept-Charset";
    constexpr const char* accept_encoding = "Accept-Encoding";
    constexpr const char* accept_language = "Accept-Language";
    constexpr const char* authorization = "Authorization";
    constexpr const char* cookie = "Cookie";
    constexpr const char* expect = "Expect";
    constexpr const char* from = "From";
    constexpr const char* host = "Host";
    constexpr const char* if_match = "If-Match";
    constexpr const char* if_modified_since = "If-Modified-Since";
    constexpr const char* if_none_match = "If-None-Match";
    constexpr const char* if_range = "If-Range";
    constexpr const char* if_unmodified_since = "If-Unmodified-Since";
    constexpr const char* max_forwards = "Max-Forwards";
    constexpr const char* proxy_authorization = "Proxy-Authorization";
    constexpr const char* range = "Range";
    constexpr const char* referer = "Referer";
    constexpr const char* te = "TE";
    constexpr const char* user_agent = "User-Agent";
    
    constexpr const char* accept_ranges = "Accept-Ranges";
    constexpr const char* age = "Age";
    constexpr const char* etag = "ETag";
    constexpr const char* location = "Location";
    constexpr const char* proxy_authenticate = "Proxy-Authenticate";
    constexpr const char* retry_after = "Retry-After";
    constexpr const char* server = "Server";
    constexpr const char* set_cookie = "Set-Cookie";
    constexpr const char* vary = "Vary";
    constexpr const char* www_authenticate = "WWW-Authenticate";
    
    constexpr const char* allow = "Allow";
    constexpr const char* content_encoding = "Content-Encoding";
    constexpr const char* content_language = "Content-Language";
    constexpr const char* content_length = "Content-Length";
    constexpr const char* content_location = "Content-Location";
    constexpr const char* content_md5 = "Content-MD5";
    constexpr const char* content_range = "Content-Range";
    constexpr const char* content_type = "Content-Type";
    constexpr const char* expires = "Expires";
    constexpr const char* last_modified = "Last-Modified";
    
    constexpr const char* access_control_allow_origin = "Access-Control-Allow-Origin";
    constexpr const char* access_control_allow_credentials = "Access-Control-Allow-Credentials";
    constexpr const char* access_control_allow_headers = "Access-Control-Allow-Headers";
    constexpr const char* access_control_allow_methods = "Access-Control-Allow-Methods";
    constexpr const char* access_control_expose_headers = "Access-Control-Expose-Headers";
    constexpr const char* access_control_max_age = "Access-Control-Max-Age";
    constexpr const char* access_control_request_headers = "Access-Control-Request-Headers";
    constexpr const char* access_control_request_method = "Access-Control-Request-Method";
    constexpr const char* origin = "Origin";
    
    constexpr const char* strict_transport_security = "Strict-Transport-Security";
    constexpr const char* x_content_type_options = "X-Content-Type-Options";
    constexpr const char* x_frame_options = "X-Frame-Options";
    constexpr const char* x_xss_protection = "X-XSS-Protection";
    constexpr const char* content_security_policy = "Content-Security-Policy";
    constexpr const char* referrer_policy = "Referrer-Policy";
    constexpr const char* feature_policy = "Feature-Policy";
    
    constexpr const char* sec_fetch_site = "Sec-Fetch-Site";
    constexpr const char* sec_fetch_mode = "Sec-Fetch-Mode";
    constexpr const char* sec_fetch_user = "Sec-Fetch-User";
    constexpr const char* sec_fetch_dest = "Sec-Fetch-Dest";
    constexpr const char* service_worker_navigation_preload = "Service-Worker-Navigation-Preload";
}

namespace http_options {
    constexpr const char* protocol = "HTTP";
    constexpr const char* version_1_1 = "1.1";
}

namespace status_codes {
    namespace informational_1xx {
        constexpr std::pair<uint16_t, const char*> continue_100 = std::make_pair(100, "Continue");
        constexpr std::pair<uint16_t, const char*> switching_protocols_101 = std::make_pair(101, "Switching Protocols");
        constexpr std::pair<uint16_t, const char*> processing_102 = std::make_pair(102, "Processing");
        constexpr std::pair<uint16_t, const char*> early_hints_103 = std::make_pair(103, "Early Hints");
    }
    
    namespace success_2xx {
        constexpr std::pair<uint16_t, const char*> ok_200 = std::make_pair(200, "OK");
        constexpr std::pair<uint16_t, const char*> created_201 = std::make_pair(201, "Created");
        constexpr std::pair<uint16_t, const char*> accepted_202 = std::make_pair(202, "Accepted");
        constexpr std::pair<uint16_t, const char*> non_authoritative_information_203 = std::make_pair(203, "Non-Authoritative Information");
        constexpr std::pair<uint16_t, const char*> no_content_204 = std::make_pair(204, "No Content");
        constexpr std::pair<uint16_t, const char*> reset_content_205 = std::make_pair(205, "Reset Content");
        constexpr std::pair<uint16_t, const char*> partial_content_206 = std::make_pair(206, "Partial Content");
        constexpr std::pair<uint16_t, const char*> multi_status_207 = std::make_pair(207, "Multi-Status");
        constexpr std::pair<uint16_t, const char*> already_reported_208 = std::make_pair(208, "Already Reported");
        constexpr std::pair<uint16_t, const char*> im_used_226 = std::make_pair(226, "IM Used");
    }
    
    namespace redirection_3xx {
        constexpr std::pair<uint16_t, const char*> multiple_choices_300 = std::make_pair(300, "Multiple Choices");
        constexpr std::pair<uint16_t, const char*> moved_permanently_301 = std::make_pair(301, "Moved Permanently");
        constexpr std::pair<uint16_t, const char*> found_302 = std::make_pair(302, "Found");
        constexpr std::pair<uint16_t, const char*> see_other_303 = std::make_pair(303, "See Other");
        constexpr std::pair<uint16_t, const char*> not_modified_304 = std::make_pair(304, "Not Modified");
        constexpr std::pair<uint16_t, const char*> use_proxy_305 = std::make_pair(305, "Use Proxy");
        constexpr std::pair<uint16_t, const char*> temporary_redirect_307 = std::make_pair(307, "Temporary Redirect");
        constexpr std::pair<uint16_t, const char*> permanent_redirect_308 = std::make_pair(308, "Permanent Redirect");
    }
    
    namespace client_errors_4xx {
        constexpr std::pair<uint16_t, const char*> bad_request_400 = std::make_pair(400, "Bad Request");
        constexpr std::pair<uint16_t, const char*> unauthorized_401 = std::make_pair(401, "Unauthorized");
        constexpr std::pair<uint16_t, const char*> payment_required_402 = std::make_pair(402, "Payment Required");
        constexpr std::pair<uint16_t, const char*> forbidden_403 = std::make_pair(403, "Forbidden");
        constexpr std::pair<uint16_t, const char*> not_found_404 = std::make_pair(404, "Not Found");
        constexpr std::pair<uint16_t, const char*> method_not_allowed_405 = std::make_pair(405, "Method Not Allowed");
        constexpr std::pair<uint16_t, const char*> not_acceptable_406 = std::make_pair(406, "Not Acceptable");
        constexpr std::pair<uint16_t, const char*> proxy_authentication_required_407 = std::make_pair(407, "Proxy Authentication Required");
        constexpr std::pair<uint16_t, const char*> request_timeout_408 = std::make_pair(408, "Request Timeout");
        constexpr std::pair<uint16_t, const char*> conflict_409 = std::make_pair(409, "Conflict");
        constexpr std::pair<uint16_t, const char*> gone_410 = std::make_pair(410, "Gone");
        constexpr std::pair<uint16_t, const char*> length_required_411 = std::make_pair(411, "Length Required");
        constexpr std::pair<uint16_t, const char*> precondition_failed_412 = std::make_pair(412, "Precondition Failed");
        constexpr std::pair<uint16_t, const char*> payload_too_large_413 = std::make_pair(413, "Payload Too Large");
        constexpr std::pair<uint16_t, const char*> uri_too_long_414 = std::make_pair(414, "URI Too Long");
        constexpr std::pair<uint16_t, const char*> unsupported_media_type_415 = std::make_pair(415, "Unsupported Media Type");
        constexpr std::pair<uint16_t, const char*> range_not_satisfiable_416 = std::make_pair(416, "Range Not Satisfiable");
        constexpr std::pair<uint16_t, const char*> expectation_failed_417 = std::make_pair(417, "Expectation Failed");
        constexpr std::pair<uint16_t, const char*> misdirected_request_421 = std::make_pair(421, "Misdirected Request");
        constexpr std::pair<uint16_t, const char*> unprocessable_entity_422 = std::make_pair(422, "Unprocessable Entity");
        constexpr std::pair<uint16_t, const char*> locked_423 = std::make_pair(423, "Locked");
        constexpr std::pair<uint16_t, const char*> failed_dependency_424 = std::make_pair(424, "Failed Dependency");
        constexpr std::pair<uint16_t, const char*> too_early_425 = std::make_pair(425, "Too Early");
        constexpr std::pair<uint16_t, const char*> upgrade_required_426 = std::make_pair(426, "Upgrade Required");
        constexpr std::pair<uint16_t, const char*> precondition_required_428 = std::make_pair(428, "Precondition Required");
        constexpr std::pair<uint16_t, const char*> too_many_requests_429 = std::make_pair(429, "Too Many Requests");
        constexpr std::pair<uint16_t, const char*> request_header_fields_too_large_431 = std::make_pair(431, "Request Header Fields Too Large");
        constexpr std::pair<uint16_t, const char*> unavailable_for_legal_reasons_451 = std::make_pair(451, "Unavailable For Legal Reasons");
    }
    namespace server_errors_5xx {
        constexpr std::pair<uint16_t, const char*> internal_server_error_500 = std::make_pair(500, "Internal Server Error");
        constexpr std::pair<uint16_t, const char*> not_implemented_501 = std::make_pair(501, "Not Implemented");
        constexpr std::pair<uint16_t, const char*> bad_gateway_502 = std::make_pair(502, "Bad Gateway");
        constexpr std::pair<uint16_t, const char*> service_unavailable_503 = std::make_pair(503, "Service Unavailable");
        constexpr std::pair<uint16_t, const char*> gateway_timeout_504 = std::make_pair(504, "Gateway Timeout");
        constexpr std::pair<uint16_t, const char*> http_version_not_supported_505 = std::make_pair(505, "HTTP Version Not Supported");
        constexpr std::pair<uint16_t, const char*> variant_also_negotiates_506 = std::make_pair(506, "Variant Also Negotiates");
        constexpr std::pair<uint16_t, const char*> insufficient_storage_507 = std::make_pair(507, "Insufficient Storage");
        constexpr std::pair<uint16_t, const char*> loop_detected_508 = std::make_pair(508, "Loop Detected");
        constexpr std::pair<uint16_t, const char*> not_extended_510 = std::make_pair(510, "Not Extended");
        constexpr std::pair<uint16_t, const char*> network_authentication_required_511 = std::make_pair(511, "Network Authentication Required");   
    }
}


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
        * @return json (nlohmann::json). Returns an empty json if parser can't parse the body.
        */
    inline json getJson() {
        return json_actions::parseStringJson(body);
    }
};


struct HttpResponse final : public HttpAction
{
    HttpResponse() {}
    HttpResponse(
        const std::string protocol,
        const std::string version,
        const std::pair<const uint16_t, const char *> status
    ) {
        this->protocol = protocol;
        this->version = version;
        this->status_code = status.first;
        this->status_text = status.second;
    }
    uint16_t status_code;
    std::string status_text;
};


struct HttpRequest final : public HttpAction
{
    std::string method;
    std::string uri;
    std::unordered_map<std::string, UriParamValue> path_params;
};


namespace response_templates {
    namespace {
        inline HttpResponse create404ErrorResponse() {
            std::string json_str = std::format(R"({{"status": "{}", "message": "{}"}})",
                status_codes::client_errors_4xx::not_found_404.first,
                status_codes::client_errors_4xx::not_found_404.second
            );
            HttpResponse error_404_response(http_options::protocol, http_options::version_1_1, status_codes::client_errors_4xx::not_found_404);
            error_404_response.headers[http_headers::content_length] = std::to_string(json_str.size());
            error_404_response.headers[http_headers::content_type] = "application/json; charset=utf-8";
            error_404_response.body = json_str;
            
            return error_404_response;
        }
    }

    inline const HttpResponse error_404_response = create404ErrorResponse();
}

