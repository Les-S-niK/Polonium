#pragma once

#include "http/http.hpp"
#include <cstdint>
#include <string>
#include <sys/types.h>
#include <unordered_map>


class ApiResponse
{
    public:
        std::pair<uint16_t, const char*> status_code;
        std::unordered_map<std::string, std::string> headers;
        std::string content;

        void appendHeaders(const std::pair<std::string, std::string>& to_append) {
            headers.insert(to_append);
        }
        void appendHeaders(const std::unordered_map<std::string, std::string>& to_append) {
            for(const std::pair<std::string, std::string>& header : to_append)
                headers.insert(header);
        }
        void appendHeaders(const std::string& key, const std::string& value) {
            headers.insert({key, value});
        }
};


class JsonResponse : public ApiResponse
{
    public:
        JsonResponse(const std::string& json_string, const std::pair<uint16_t, const char*>& status_code = status_codes::ok_200) {
            this->status_code = status_code;
            this->content = json_string;
        }
};
