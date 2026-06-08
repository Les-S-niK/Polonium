/**
 * @file
 * @brief Header with abstract data types that represents API responses.
 */

#pragma once

#include <cstdint>
#include <string>
#include <string_view>
#include <unordered_map>
#include <utility>

#include "polonium/dto.hpp"
#include "polonium/http/http.hpp"
#include "polonium/json_parser.hpp"

namespace polonium {
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

    auto setStatusCode(std::pair<uint16_t, const char*> status_code) noexcept
        -> void;
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
    ApiResponseEmptyContent(const ApiResponseEmptyContent&) = default;
    ApiResponseEmptyContent(ApiResponseEmptyContent&&) = delete;
    auto operator=(const ApiResponseEmptyContent&)
        -> ApiResponseEmptyContent& = default;
    auto operator=(ApiResponseEmptyContent&&)
        -> ApiResponseEmptyContent& = delete;
    ~ApiResponseEmptyContent() override = default;

    auto getContent() const -> std::string override;
};

class JsonResponse final : public ApiResponseWithContent {
   public:
    explicit JsonResponse(const std::pair<uint16_t, const char*>& status_code =
                              status_codes::success_2xx::ok_200);
    explicit JsonResponse(json_actions::json&& content,
                          const std::pair<uint16_t, const char*>& status_code =
                              status_codes::success_2xx::ok_200);

    JsonResponse(const JsonResponse&) = default;
    JsonResponse(JsonResponse&&) noexcept = default;
    auto operator=(const JsonResponse&) -> JsonResponse& = default;
    auto operator=(JsonResponse&&) noexcept -> JsonResponse& = default;
    ~JsonResponse() override = default;

    template <polonium::dto::is_trivially_aggregate Aggregate>
    auto setContent(Aggregate object) {
        content_ = polonium::dto::toJson(std::move(object));
    }
    auto setContent(std::string_view content) -> void override;
    auto setContent(std::string&& content) -> void override;

    template <polonium::dto::is_trivially_aggregate Aggregate>
    auto appendContent(Aggregate object) {
        content_.merge(polonium::dto::toJson(std::move(object)));
    }

    auto appendContent(const std::string&& key, const std::string&& value)
        -> void;

    auto getContent() const -> std::string override;

   private:
    json_actions::json content_;
};
}  // namespace polonium
