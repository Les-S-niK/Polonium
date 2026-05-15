
#pragma once

#include <cstdint>
#include <utility>

namespace polonium {
namespace http_headers {
inline constexpr const char* cache_control = "Cache-Control";
inline constexpr const char* connection = "Connection";
inline constexpr const char* date = "Date";
inline constexpr const char* pragma = "Pragma";
inline constexpr const char* trailer = "Trailer";
inline constexpr const char* transfer_encoding = "Transfer-Encoding";
inline constexpr const char* upgrade = "Upgrade";
inline constexpr const char* via = "Via";
inline constexpr const char* warning = "Warning";

inline constexpr const char* accept = "Accept";
inline constexpr const char* accept_charset = "Accept-Charset";
inline constexpr const char* accept_encoding = "Accept-Encoding";
inline constexpr const char* accept_language = "Accept-Language";
inline constexpr const char* authorization = "Authorization";
inline constexpr const char* cookie = "Cookie";
inline constexpr const char* expect = "Expect";
inline constexpr const char* from = "From";
inline constexpr const char* host = "Host";
inline constexpr const char* if_match = "If-Match";
inline constexpr const char* if_modified_since = "If-Modified-Since";
inline constexpr const char* if_none_match = "If-None-Match";
inline constexpr const char* if_range = "If-Range";
inline constexpr const char* if_unmodified_since = "If-Unmodified-Since";
inline constexpr const char* max_forwards = "Max-Forwards";
inline constexpr const char* proxy_authorization = "Proxy-Authorization";
inline constexpr const char* range = "Range";
inline constexpr const char* referer = "Referer";
inline constexpr const char* te = "TE";  // NOLINT: correct header field.
inline constexpr const char* user_agent = "User-Agent";

inline constexpr const char* accept_ranges = "Accept-Ranges";
inline constexpr const char* age = "Age";
inline constexpr const char* etag = "ETag";
inline constexpr const char* location = "Location";
inline constexpr const char* proxy_authenticate = "Proxy-Authenticate";
inline constexpr const char* retry_after = "Retry-After";
inline constexpr const char* server = "Server";
inline constexpr const char* set_cookie = "Set-Cookie";
inline constexpr const char* vary = "Vary";
inline constexpr const char* www_authenticate = "WWW-Authenticate";

inline constexpr const char* allow = "Allow";
inline constexpr const char* content_encoding = "Content-Encoding";
inline constexpr const char* content_language = "Content-Language";
inline constexpr const char* content_length = "Content-Length";
inline constexpr const char* content_location = "Content-Location";
inline constexpr const char* content_md5 = "Content-MD5";
inline constexpr const char* content_range = "Content-Range";
inline constexpr const char* content_type = "Content-Type";
inline constexpr const char* expires = "Expires";
inline constexpr const char* last_modified = "Last-Modified";

inline constexpr const char* access_control_allow_origin =
    "Access-Control-Allow-Origin";
inline constexpr const char* access_control_allow_credentials =
    "Access-Control-Allow-Credentials";
inline constexpr const char* access_control_allow_headers =
    "Access-Control-Allow-Headers";
inline constexpr const char* access_control_allow_methods =
    "Access-Control-Allow-Methods";
inline constexpr const char* access_control_expose_headers =
    "Access-Control-Expose-Headers";
inline constexpr const char* access_control_max_age = "Access-Control-Max-Age";
inline constexpr const char* access_control_request_headers =
    "Access-Control-Request-Headers";
inline constexpr const char* access_control_request_method =
    "Access-Control-Request-Method";
inline constexpr const char* origin = "Origin";

inline constexpr const char* strict_transport_security =
    "Strict-Transport-Security";
inline constexpr const char* x_content_type_options = "X-Content-Type-Options";
inline constexpr const char* x_frame_options = "X-Frame-Options";
inline constexpr const char* x_xss_protection = "X-XSS-Protection";
inline constexpr const char* content_security_policy =
    "Content-Security-Policy";
inline constexpr const char* referrer_policy = "Referrer-Policy";
inline constexpr const char* feature_policy = "Feature-Policy";

inline constexpr const char* sec_fetch_site = "Sec-Fetch-Site";
inline constexpr const char* sec_fetch_mode = "Sec-Fetch-Mode";
inline constexpr const char* sec_fetch_user = "Sec-Fetch-User";
inline constexpr const char* sec_fetch_dest = "Sec-Fetch-Dest";
inline constexpr const char* service_worker_navigation_preload =
    "Service-Worker-Navigation-Preload";
}  // namespace http_headers

namespace http_options {
inline constexpr const char* protocol = "HTTP";
inline constexpr const char* version_1_1 = "1.1";
}  // namespace http_options

namespace status_codes {
namespace informational_1xx {
inline constexpr std::pair<uint16_t, const char*> continue_100 =
    std::make_pair(100, "Continue");
inline constexpr std::pair<uint16_t, const char*> switching_protocols_101 =
    std::make_pair(101, "Switching Protocols");
inline constexpr std::pair<uint16_t, const char*> processing_102 =
    std::make_pair(102, "Processing");
inline constexpr std::pair<uint16_t, const char*> early_hints_103 =
    std::make_pair(103, "Early Hints");
}  // namespace informational_1xx

namespace success_2xx {
inline constexpr std::pair<uint16_t, const char*> ok_200 =
    std::make_pair(200, "OK");
inline constexpr std::pair<uint16_t, const char*> created_201 =
    std::make_pair(201, "Created");
inline constexpr std::pair<uint16_t, const char*> accepted_202 =
    std::make_pair(202, "Accepted");
inline constexpr std::pair<uint16_t, const char*>
    non_authoritative_information_203 =
        std::make_pair(203, "Non-Authoritative Information");
inline constexpr std::pair<uint16_t, const char*> no_content_204 =
    std::make_pair(204, "No Content");
inline constexpr std::pair<uint16_t, const char*> reset_content_205 =
    std::make_pair(205, "Reset Content");
inline constexpr std::pair<uint16_t, const char*> partial_content_206 =
    std::make_pair(206, "Partial Content");
inline constexpr std::pair<uint16_t, const char*> multi_status_207 =
    std::make_pair(207, "Multi-Status");
inline constexpr std::pair<uint16_t, const char*> already_reported_208 =
    std::make_pair(208, "Already Reported");
inline constexpr std::pair<uint16_t, const char*> im_used_226 =
    std::make_pair(226, "IM Used");
}  // namespace success_2xx

namespace redirection_3xx {
inline constexpr std::pair<uint16_t, const char*> multiple_choices_300 =
    std::make_pair(300, "Multiple Choices");
inline constexpr std::pair<uint16_t, const char*> moved_permanently_301 =
    std::make_pair(301, "Moved Permanently");
inline constexpr std::pair<uint16_t, const char*> found_302 =
    std::make_pair(302, "Found");
inline constexpr std::pair<uint16_t, const char*> see_other_303 =
    std::make_pair(303, "See Other");
inline constexpr std::pair<uint16_t, const char*> not_modified_304 =
    std::make_pair(304, "Not Modified");
inline constexpr std::pair<uint16_t, const char*> use_proxy_305 =
    std::make_pair(305, "Use Proxy");
inline constexpr std::pair<uint16_t, const char*> temporary_redirect_307 =
    std::make_pair(307, "Temporary Redirect");
inline constexpr std::pair<uint16_t, const char*> permanent_redirect_308 =
    std::make_pair(308, "Permanent Redirect");
}  // namespace redirection_3xx

namespace client_errors_4xx {
inline constexpr std::pair<uint16_t, const char*> bad_request_400 =
    std::make_pair(400, "Bad Request");
inline constexpr std::pair<uint16_t, const char*> unauthorized_401 =
    std::make_pair(401, "Unauthorized");
inline constexpr std::pair<uint16_t, const char*> payment_required_402 =
    std::make_pair(402, "Payment Required");
inline constexpr std::pair<uint16_t, const char*> forbidden_403 =
    std::make_pair(403, "Forbidden");
inline constexpr std::pair<uint16_t, const char*> not_found_404 =
    std::make_pair(404, "Not Found");
inline constexpr std::pair<uint16_t, const char*> method_not_allowed_405 =
    std::make_pair(405, "Method Not Allowed");
inline constexpr std::pair<uint16_t, const char*> not_acceptable_406 =
    std::make_pair(406, "Not Acceptable");
inline constexpr std::pair<uint16_t, const char*>
    proxy_authentication_required_407 =
        std::make_pair(407, "Proxy Authentication Required");
inline constexpr std::pair<uint16_t, const char*> request_timeout_408 =
    std::make_pair(408, "Request Timeout");
inline constexpr std::pair<uint16_t, const char*> conflict_409 =
    std::make_pair(409, "Conflict");
inline constexpr std::pair<uint16_t, const char*> gone_410 =
    std::make_pair(410, "Gone");
inline constexpr std::pair<uint16_t, const char*> length_required_411 =
    std::make_pair(411, "Length Required");
inline constexpr std::pair<uint16_t, const char*> precondition_failed_412 =
    std::make_pair(412, "Precondition Failed");
inline constexpr std::pair<uint16_t, const char*> payload_too_large_413 =
    std::make_pair(413, "Payload Too Large");
inline constexpr std::pair<uint16_t, const char*> uri_too_long_414 =
    std::make_pair(414, "URI Too Long");
inline constexpr std::pair<uint16_t, const char*> unsupported_media_type_415 =
    std::make_pair(415, "Unsupported Media Type");
inline constexpr std::pair<uint16_t, const char*> range_not_satisfiable_416 =
    std::make_pair(416, "Range Not Satisfiable");
inline constexpr std::pair<uint16_t, const char*> expectation_failed_417 =
    std::make_pair(417, "Expectation Failed");
inline constexpr std::pair<uint16_t, const char*> misdirected_request_421 =
    std::make_pair(421, "Misdirected Request");
inline constexpr std::pair<uint16_t, const char*> unprocessable_entity_422 =
    std::make_pair(422, "Unprocessable Entity");
inline constexpr std::pair<uint16_t, const char*> locked_423 =
    std::make_pair(423, "Locked");
inline constexpr std::pair<uint16_t, const char*> failed_dependency_424 =
    std::make_pair(424, "Failed Dependency");
inline constexpr std::pair<uint16_t, const char*> too_early_425 =
    std::make_pair(425, "Too Early");
inline constexpr std::pair<uint16_t, const char*> upgrade_required_426 =
    std::make_pair(426, "Upgrade Required");
inline constexpr std::pair<uint16_t, const char*> precondition_required_428 =
    std::make_pair(428, "Precondition Required");
inline constexpr std::pair<uint16_t, const char*> too_many_requests_429 =
    std::make_pair(429, "Too Many Requests");
inline constexpr std::pair<uint16_t, const char*>
    request_header_fields_too_large_431 =
        std::make_pair(431, "Request Header Fields Too Large");
inline constexpr std::pair<uint16_t, const char*>
    unavailable_for_legal_reasons_451 =
        std::make_pair(451, "Unavailable For Legal Reasons");
}  // namespace client_errors_4xx
namespace server_errors_5xx {
inline constexpr std::pair<uint16_t, const char*> internal_server_error_500 =
    std::make_pair(500, "Internal Server Error");
inline constexpr std::pair<uint16_t, const char*> not_implemented_501 =
    std::make_pair(501, "Not Implemented");
inline constexpr std::pair<uint16_t, const char*> bad_gateway_502 =
    std::make_pair(502, "Bad Gateway");
inline constexpr std::pair<uint16_t, const char*> service_unavailable_503 =
    std::make_pair(503, "Service Unavailable");
inline constexpr std::pair<uint16_t, const char*> gateway_timeout_504 =
    std::make_pair(504, "Gateway Timeout");
inline constexpr std::pair<uint16_t, const char*>
    http_version_not_supported_505 =
        std::make_pair(505, "HTTP Version Not Supported");
inline constexpr std::pair<uint16_t, const char*> variant_also_negotiates_506 =
    std::make_pair(506, "Variant Also Negotiates");
inline constexpr std::pair<uint16_t, const char*> insufficient_storage_507 =
    std::make_pair(507, "Insufficient Storage");
inline constexpr std::pair<uint16_t, const char*> loop_detected_508 =
    std::make_pair(508, "Loop Detected");
inline constexpr std::pair<uint16_t, const char*> not_extended_510 =
    std::make_pair(510, "Not Extended");
inline constexpr std::pair<uint16_t, const char*>
    network_authentication_required_511 =
        std::make_pair(511, "Network Authentication Required");
}  // namespace server_errors_5xx
}  // namespace status_codes
};  // namespace polonium
