
#include "polonium/routing/api_responses.hpp"

#include "polonium/json_parser.hpp"

polonium::ApiResponse::ApiResponse(
    std::pair<uint16_t, const char*> status_code,
    std::unordered_map<std::string, std::string> headers)
    : status_code_(std::move(status_code)), headers_(std::move(headers)) {}

auto polonium::ApiResponse::getStatusCode() const noexcept
    -> std::pair<uint16_t, const char*> {
    return status_code_;
}
auto polonium::ApiResponse::setStatusCode(
    std::pair<uint16_t, const char*> status_code) noexcept -> void {
    status_code_ = status_code;
}
auto polonium::ApiResponse::getHeaders() const noexcept
    -> std::unordered_map<std::string, std::string> {
    return headers_;
}
auto polonium::ApiResponse::appendHeaders(
    std::pair<std::string, std::string>&& to_append) -> void {
    headers_.insert(std::move(to_append));
}
auto polonium::ApiResponse::appendHeaders(
    std::unordered_map<std::string, std::string>&& to_append) -> void {
    for (auto&& header : std::move(to_append)) {
        headers_.insert(std::move(header));
    }
}
auto polonium::ApiResponse::appendHeaders(std::string&& key,
                                          std::string&& value) -> void {
    headers_.insert({std::move(key), std::move(value)});
}

polonium::JsonResponse::JsonResponse(
    const std::pair<uint16_t, const char*>& status_code) {
    setStatusCode(status_code);
}
polonium::JsonResponse::JsonResponse(
    json_actions::json&& content,
    const std::pair<uint16_t, const char*>& status_code)
    : JsonResponse(status_code) {
    content_ = std::move(content);
}

auto polonium::JsonResponse::setContent(std::string_view content) -> void {
    content_ = json_actions::stringToJson(content);
}
auto polonium::JsonResponse::setContent(std::string&& content) -> void {
    content_ = json_actions::stringToJson(std::move(content));
}
auto polonium::JsonResponse::getContent() const -> std::string {
    return json_actions::jsonToString(content_);
}
auto polonium::JsonResponse::appendContent(const std::string&& key,
                                           const std::string&& value) -> void {
    content_.emplace(key, value);
}

polonium::ApiResponseEmptyContent::ApiResponseEmptyContent(
    const std::pair<uint16_t, const char*>& status_code) {
    setStatusCode(status_code);
}
auto polonium::ApiResponseEmptyContent::getContent() const -> std::string {
    return {};
}
