#pragma once

#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <string>
#include <string_view>

#include "polonium/http/http.hpp"
#include "polonium/polonium_logger.hpp"

inline constexpr const char* gmt_time_format = "%a, %d %b %Y %H:%M:%S GMT";

class HttpResponseSerializer {
   public:
    HttpResponseSerializer(const HttpResponseSerializer&) = delete;
    HttpResponseSerializer(HttpResponseSerializer&&) = delete;
    auto operator=(const HttpResponseSerializer&)
        -> HttpResponseSerializer = delete;
    auto operator=(HttpResponseSerializer&&) -> HttpResponseSerializer = delete;
    HttpResponseSerializer(PoloniumLogger& logger, const HttpResponse& response)
        : logger_(logger), response_(response) {}

    [[nodiscard]] auto serializeResponse() const -> std::string {
        logger_.trace(__func__);
        std::string response_buffer;
        constexpr size_t default_response_buffer_size = 2048;
        response_buffer.reserve(default_response_buffer_size);

        response_buffer =
            std::format("{}/{} {} {}\r\n", http_options::protocol,
                        http_options::version_1_1, response_.status_code,
                        response_.status_text);

        for (const std::pair<const std::string_view, std::string_view> header :
             response_.headers) {
            response_buffer.append(
                std::format("{}: {}\r\n", header.first, header.second));
        }
        response_buffer.append(std::format("{}: {}\r\n\r\n", http_headers::date,
                                           getCurrentGmtTime()));
        response_buffer.append(response_.body);

        return response_buffer;
    }

    static auto getCurrentGmtTime() -> std::string {
        using namespace std::chrono;
        std::stringstream str_stream;

        auto current_time = system_clock::now();
        time_t time = system_clock::to_time_t(current_time);
        str_stream << std::put_time(std::gmtime(&time), gmt_time_format);

        return str_stream.str();
    }

   private:
    PoloniumLogger& logger_;
    const HttpResponse& response_;
    std::string http_response_;
};
