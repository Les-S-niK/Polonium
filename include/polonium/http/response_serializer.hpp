#pragma once

#include <ctime>
#include <string>

#include "polonium/app/polonium_logger.hpp"
#include "polonium/http/http_actions.hpp"

namespace polonium {
/**
 * @class HttpResponseSerializer
 * @brief Class uses to Serialize HttpResponse instances to string.
 *
 */
class HttpResponseSerializer {
   public:
    explicit HttpResponseSerializer(HttpResponse&& response);
    HttpResponseSerializer(const HttpResponseSerializer&) = delete;
    HttpResponseSerializer(HttpResponseSerializer&&) = delete;
    auto operator=(const HttpResponseSerializer&)
        -> HttpResponseSerializer = delete;
    auto operator=(HttpResponseSerializer&&) -> HttpResponseSerializer = delete;
    ~HttpResponseSerializer() = default;

    /**
     * @brief serialize HTTP response headers and body to a string.
     *
     * @return std::string: serialized to a string HTTP response.
     */
    [[nodiscard]] auto serializeResponse() const -> std::string;
    /**
     * @brief get current GMT time and return it as a string.
     *
     * @return std::string: GMT time in string format.
     */
    static auto getCurrentGmtTime() -> std::string;

   private:
    polonium::PoloniumLogger* logger_;
    HttpResponse response_;
    std::string http_response_;
};
}  // namespace polonium
