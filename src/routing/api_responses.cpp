
#include "polonium/routing/api_responses.hpp"

ApiResponse::ApiResponse(std::pair<uint16_t, const char*> status_code,
                         std::unordered_map<std::string, std::string> headers)
    : status_code_(std::move(status_code)), headers_(std::move(headers)) {}

auto ApiResponse::getStatusCode() const noexcept
    -> std::pair<uint16_t, const char*> {
    return status_code_;
}
auto ApiResponse::setStatusCode(
    std::pair<uint16_t, const char*> status_code) noexcept {
    status_code_ = status_code;
}
auto ApiResponse::getHeaders() const noexcept
    -> std::unordered_map<std::string, std::string> {
    return headers_;
}
auto ApiResponse::appendHeaders(std::pair<std::string, std::string>&& to_append)
    -> void {
    headers_.insert(to_append);
}
auto ApiResponse::appendHeaders(
    std::unordered_map<std::string, std::string>&& to_append) -> void {
    for (const auto& header : to_append) {
        headers_.insert(header);
    }
}
auto ApiResponse::appendHeaders(std::string&& key, std::string&& value)
    -> void {
    headers_.insert({std::move(key), std::move(value)});
}

JsonResponse::JsonResponse(
    const std::pair<uint16_t, const char*>& status_code) {
    setStatusCode(status_code);
}
auto JsonResponse::setContent(std::string_view content) -> void {
    content_ = json_actions::parseStringJson(content);
}
auto JsonResponse::setContent(std::string&& content) -> void {
    content_ = json_actions::parseStringJson(std::move(content));
}
auto JsonResponse::getContent() const -> std::string { return content_.dump(); }
auto JsonResponse::appendContent(const std::string&& key,
                                 const std::string&& value) -> void {
    content_.emplace(key, value);
}
