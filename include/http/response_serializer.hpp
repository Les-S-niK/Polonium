#pragma once

#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <string>
#include <string_view>

#include "http.hpp"
#include "polonium_logger.hpp"


// TODO: Implement the first version of the serializer logic. 
inline constexpr const char* gmt_time_format = "%a, %d %b %Y %H:%M:%S GMT";


class HttpResponseSerializer
{
    public:
        HttpResponseSerializer(const HttpResponseSerializer&) = delete;
        HttpResponseSerializer(HttpResponseSerializer&&) = delete;
        HttpResponseSerializer operator=(const HttpResponseSerializer&) = delete;
        HttpResponseSerializer operator=(HttpResponseSerializer&&) = delete;
        HttpResponseSerializer(PoloniumLogger& logger, HttpResponse& response):
            logger_(logger),
            response_(response)
        {}

        std::string serializeResponse() {
            std::string response_buffer;
            response_buffer.reserve(2048);

            response_buffer = std::format(
                "{}/{} {} {}\r\n",
                http_options::protocol,
                http_options::version_1_1,
                response_.status_code,
                response_.status_text
            );

            for(const std::pair<const std::string_view, std::string_view>& header : response_.headers)
                response_buffer.append(std::format("{}: {}\r\n", header.first, header.second));
            response_buffer.append(std::format("{}: {}\r\n\r\n", http_headers::date, getCurrentGmtTime()));
            response_buffer.append(response_.body);

            return response_buffer;
        }

        inline static std::string getCurrentGmtTime() {
            using namespace std::chrono;
            std::stringstream str_stream;

            auto current_time = system_clock::now();
            time_t time = system_clock::to_time_t(current_time);            
            str_stream << std::put_time(std::gmtime(&time), gmt_time_format);

            return str_stream.str();
        }

    
    private:
        PoloniumLogger& logger_;
        HttpResponse& response_;
        std::string http_response_;
};
