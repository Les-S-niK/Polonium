/**
 * @file
 * @brief Header with abstract data types that represents API responses.
 */

#pragma once

#include <stdexcept>
#include <string>
#include <string_view>
#include <unordered_map>
#include <utility>

#include "polonium/http/http.hpp"
#include "polonium/json_parser.hpp"

/**
 * @class ApiResponse
 * @brief Base API response abstract data type. Any other class that represents
 * response from API must inherit this class.
 *
 */
class ApiResponse {
   public:
    ApiResponse() = default;
    ApiResponse(std::pair<uint16_t, const char*> status_code,
                std::unordered_map<std::string, std::string> headers)
        : status_code_(std::move(status_code)), headers_(std::move(headers)) {}
    ApiResponse(const ApiResponse&) = default;
    ApiResponse(ApiResponse&&) = delete;
    auto operator=(const ApiResponse&) -> ApiResponse& = default;
    auto operator=(ApiResponse&&) -> ApiResponse& = delete;
    virtual ~ApiResponse() = default;

    virtual auto getContent() const -> std::string = 0;
    virtual auto setContent(std::string_view content) -> void = 0;
    virtual auto setContent(std::string&& content) -> void = 0;
    auto getStatusCode() const noexcept -> std::pair<uint16_t, const char*> {
        return status_code_;
    }
    auto setStatusCode(std::pair<uint16_t, const char*> status_code) noexcept {
        status_code_ = status_code;
    }
    auto getHeaders() const noexcept
        -> std::unordered_map<std::string, std::string> {
        return headers_;
    }
    auto appendHeaders(const std::pair<std::string, std::string>&& to_append)
        -> void {
        headers_.insert(to_append);
    }
    auto appendHeaders(
        const std::unordered_map<std::string, std::string>&& to_append)
        -> void {
        for (const auto& header : to_append) {
            headers_.insert(header);
        }
    }
    auto appendHeaders(std::string&& key, std::string&& value) -> void {
        headers_.insert({std::move(key), std::move(value)});
    }

   private:
    std::pair<uint16_t, const char*> status_code_;
    std::string content_;
    std::unordered_map<std::string, std::string> headers_;
};

class JsonResponse final : public ApiResponse {
   public:
    explicit JsonResponse(const std::pair<uint16_t, const char*>& status_code =
                              status_codes::success_2xx::ok_200) {
        setStatusCode(status_code);
    }
    auto setContent(std::string_view content) -> void override {
        content_ = json_actions::parseStringJson(content);
    }
    auto setContent(std::string&& content) -> void override {
        content_ = json_actions::parseStringJson(std::move(content));
    }
    auto getContent() const -> std::string override { return content_.dump(); }
    auto appendContent(const std::string&& key, const std::string&& value)
        -> void {
        content_.emplace(key, value);
    }

   private:
    json content_;
};

class NoResponse final : public ApiResponse {
   public:
    explicit NoResponse(const std::pair<uint16_t, const char*>& status_code =
                            status_codes::success_2xx::ok_200) {
        setStatusCode(status_code);
    }
    auto setContent([[maybe_unused]] std::string_view /*unused*/)
        -> void override {
        throwCanNotSetContent();
    }
    auto setContent([[maybe_unused]] std::string&& /*unused*/)
        -> void override {
        throwCanNotSetContent();
    }
    auto getContent() const -> std::string override { return {}; }

   private:
    static auto throwCanNotSetContent() -> void {
        throw std::runtime_error(
            "Incorrect action. Can not set content for this object. ");
    }
};
