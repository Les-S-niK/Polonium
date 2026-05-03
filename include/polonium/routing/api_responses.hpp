/**
 * @file
 * @brief Header with abstract data types that represents API responses.
 */

#pragma once

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
                std::unordered_map<std::string, std::string> headers);
    ApiResponse(const ApiResponse&) = default;
    ApiResponse(ApiResponse&&) = delete;
    auto operator=(const ApiResponse&) -> ApiResponse& = default;
    auto operator=(ApiResponse&&) -> ApiResponse& = delete;
    virtual ~ApiResponse() = default;

    virtual auto getContent() const -> std::string = 0;

    auto setStatusCode(std::pair<uint16_t, const char*> status_code) noexcept;
    auto appendHeaders(std::pair<std::string, std::string>&& to_append) -> void;
    auto appendHeaders(std::unordered_map<std::string, std::string>&& to_append)
        -> void;
    auto appendHeaders(std::string&& key, std::string&& value) -> void;

    auto getStatusCode() const noexcept -> std::pair<uint16_t, const char*>;
    auto getHeaders() const noexcept
        -> std::unordered_map<std::string, std::string>;

   private:
    std::pair<uint16_t, const char*> status_code_;
    std::string content_;
    std::unordered_map<std::string, std::string> headers_;
};

class ApiResponseWithContent : public ApiResponse {
   public:
    virtual auto setContent(std::string_view content) -> void = 0;
    virtual auto setContent(std::string&& content) -> void = 0;
};

class ApiResponseEmptyContent final : public ApiResponse {
   public:
    explicit ApiResponseEmptyContent(
        const std::pair<uint16_t, const char*>& status_code =
            status_codes::success_2xx::ok_200);
    auto getContent() const -> std::string override { return {}; }
};

class JsonResponse final : public ApiResponseWithContent {
   public:
    explicit JsonResponse(const std::pair<uint16_t, const char*>& status_code =
                              status_codes::success_2xx::ok_200);

    auto setContent(std::string_view content) -> void override;
    auto setContent(std::string&& content) -> void override;
    auto appendContent(const std::string&& key, const std::string&& value)
        -> void;

    auto getContent() const -> std::string override;

   private:
    json content_;
};
