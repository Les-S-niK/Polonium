
#pragma once

#include <string_view>
#include <utility>

#include "polonium/polonium_logger.hpp"
#include "polonium/routing/dispatcher.hpp"
#include "polonium/routing/uri_parser.hpp"

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

    Route(std::string method, const std::string& uri, endpoint_handler handler)
        : method(std::move(method)), uri(uri), handler(std::move(handler)) {
        templates = UriTemplateParser(uri).getUriParamsTemplate();
    }
};

class PoloniumRouter {
   public:
    explicit PoloniumRouter(std::string_view default_uri = "")
        : main_uri(default_uri), logger_(PoloniumLogger::getInstance()) {}

    void get(const std::string& uri, endpoint_handler handler) {
        newMethod(http_methods::get, uri, std::move(handler));
    }
    void post(const std::string& uri, endpoint_handler handler) {
        newMethod(http_methods::post, uri, std::move(handler));
    }
    void put(const std::string& uri, endpoint_handler handler) {
        newMethod(http_methods::put, uri, std::move(handler));
    }
    void http_delete(const std::string& uri, endpoint_handler handler) {
        newMethod(http_methods::http_delete, uri, std::move(handler));
    }
    void patch(const std::string& uri, endpoint_handler handler) {
        newMethod(http_methods::patch, uri, std::move(handler));
    }
    void head(const std::string& uri, endpoint_handler handler) {
        newMethod(http_methods::head, uri, std::move(handler));
    }
    void options(const std::string& uri, endpoint_handler handler) {
        newMethod(http_methods::options, uri, std::move(handler));
    }
    void connect(const std::string& uri, endpoint_handler handler) {
        newMethod(http_methods::connect, uri, std::move(handler));
    }
    void trace(const std::string& uri, endpoint_handler handler) {
        newMethod(http_methods::trace, uri, std::move(handler));
    }

    void includeDispatcher(Dispatcher& dispatcher) noexcept {
        logger_->trace(__func__);
        for (auto& [method, uri, handler, templates] : routes_) {
            dispatcher.registerMethod(std::move(method), std::move(uri),
                                      std::move(handler), std::move(templates));
        }
        routes_.clear();
        logger_->debug("Methods are included in the dispatcher.");
    }

    std::string main_uri;

   private:
    std::vector<Route> routes_;
    PoloniumLogger* logger_;

    void newMethod(const char* method, const std::string& uri,
                   endpoint_handler handler) {
        logger_->trace(__func__);
        routes_.emplace_back(method, main_uri + uri, std::move(handler));
    }
};
