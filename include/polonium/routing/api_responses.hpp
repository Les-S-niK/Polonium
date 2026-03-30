#pragma once

#include "polonium/http/http.hpp"

class ApiResponse {
   public:
    ApiResponse(const ApiResponse&) = default;
    ApiResponse(ApiResponse&&) = delete;
    auto operator=(const ApiResponse&) -> ApiResponse& = default;
    auto operator=(ApiResponse&&) -> ApiResponse& = delete;
    ApiResponse() = default;
    ApiResponse(std::pair<uint16_t, const char*> status_code,
                std::unordered_map<std::string, std::string> headers,
                std::string content)
        : status_code(std::move(status_code)),
          headers(std::move(headers)),
          content(std::move(content)) {}
    virtual ~ApiResponse() = default;

    std::pair<uint16_t, const char*> status_code;
    std::unordered_map<std::string, std::string> headers;

    virtual auto getContent() -> std::string = 0;

    void appendHeaders(const std::pair<std::string, std::string>&& to_append) {
        headers.insert(to_append);
    }
    void appendHeaders(
        const std::unordered_map<std::string, std::string>&& to_append) {
        for (const auto& header : to_append) {
            headers.insert(header);
        }
    }
    void appendHeaders(std::string&& key, std::string&& value) {
        headers.insert({std::move(key), std::move(value)});
    }

   protected:
    std::string content;
};

class JsonResponse final : public ApiResponse {
   public:
    JsonResponse(const std::pair<uint16_t, const char*>& status_code =
                     status_codes::success_2xx::ok_200) {
        this->status_code = status_code;
    }

    auto getContent() -> std::string override {
        this->content = body_.dump();
        return this->content;
    }

    void appendContent(const std::string&& key, const std::string&& value) {
        body_.emplace(key, value);
    }

   private:
    json body_;
};

struct NoResponse final : public ApiResponse {
    NoResponse(const std::pair<uint16_t, const char*>& status_code =
                   status_codes::success_2xx::ok_200) {
        this->status_code = status_code;
    }
    auto getContent() -> std::string override { return {}; }
};
