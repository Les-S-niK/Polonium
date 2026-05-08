
#pragma once

#include <string_view>

#include "polonium/app/polonium_logger.hpp"
#include "polonium/routing/dispatcher.hpp"
#include "polonium/routing/uri_parser.hpp"

namespace polonium {
namespace http_methods {
constexpr const char* get = "GET";
constexpr const char* post = "POST";
constexpr const char* put = "PUT";
constexpr const char* http_delete = "DELETE";
constexpr const char* patch = "PATCH";
constexpr const char* head = "HEAD";
constexpr const char* options = "OPTIONS";
constexpr const char* connect = "CONNECT";
constexpr const char* trace = "TRACE";
}  // namespace http_methods

struct Route {
    std::string method;
    std::string uri;
    endpoint_handler handler;
    parsed_templates templates;

    Route(std::string method, std::string uri, endpoint_handler handler);
};

class PoloniumRouter {
   public:
    explicit PoloniumRouter(std::string default_uri = "");

    [[nodiscard]] auto getDefaultUri() const noexcept -> std::string;
    [[nodiscard]] auto getDefaultUriView() const noexcept -> std::string_view;
    auto setDefaultUri(std::string value) noexcept -> PoloniumRouter&;

    auto includeDispatcher(Dispatcher& dispatcher) -> void;

    auto get(const std::string& uri, endpoint_handler handler) -> void;
    auto post(const std::string& uri, endpoint_handler handler) -> void;
    auto put(const std::string& uri, endpoint_handler handler) -> void;
    auto deleteHttp(const std::string& uri, endpoint_handler handler) -> void;
    auto patch(const std::string& uri, endpoint_handler handler) -> void;
    auto head(const std::string& uri, endpoint_handler handler) -> void;
    auto options(const std::string& uri, endpoint_handler handler) -> void;
    auto connect(const std::string& uri, endpoint_handler handler) -> void;
    auto trace(const std::string& uri, endpoint_handler handler) -> void;

   private:
    std::string default_uri_;
    std::vector<Route> routes_;
    polonium::PoloniumLogger* logger_;

    auto newMethod(const char* method, const std::string& uri,
                   endpoint_handler handler) -> void;
};
}  // namespace polonium
