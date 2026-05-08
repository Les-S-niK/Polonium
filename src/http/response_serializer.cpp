
#include "polonium/http/response_serializer.hpp"

[[nodiscard]] auto polonium::HttpResponseSerializer::serializeResponse() const
    -> std::string {
    logger_->trace(__func__);
    std::string response_buffer;
    constexpr size_t default_response_buffer_size = 2048;
    response_buffer.reserve(default_response_buffer_size);

    // Serialize the first line of headers.
    response_buffer = std::format("{}/{} {} {}\r\n", http_options::protocol,
                                  http_options::version_1_1,
                                  response_.status_code, response_.status_text);

    // Serialize other headers.
    for (const std::pair<const std::string_view, std::string_view> header :
         response_.headers) {
        response_buffer.append(
            std::format("{}: {}\r\n", header.first, header.second));
    }
    response_buffer.append(
        std::format("{}: {}\r\n\r\n", http_headers::date, getCurrentGmtTime()));
    response_buffer.append(response_.body);

    return response_buffer;
}

[[nodiscard]] auto polonium::HttpResponseSerializer::getCurrentGmtTime()
    -> std::string {
    using std::chrono::system_clock;
    constexpr const char* gmt_time_format = "%a, %d %b %Y %H:%M:%S GMT";

    std::stringstream str_stream;
    auto current_time = system_clock::now();
    time_t time = system_clock::to_time_t(current_time);

    struct tm stm;
    gmtime_r(&time, &stm);
    str_stream << std::put_time(&stm, gmt_time_format);

    return str_stream.str();
}
