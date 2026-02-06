
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
}


struct Route
{
    std::string method;
    std::string uri;
    endpoint_handler handler;
    parsed_templates templates;
    
    Route(std::string method, std::string uri, endpoint_handler handler)
     :
      method(method),
      uri(uri),
      handler(handler)
    { templates = UriTemplateParser(uri).getUriParamsTemplate(); } 
};


class PoloniumRouter
{
    public:
        std::string main_uri;
        PoloniumLogger& logger_ = PoloniumLogger::getInstance();
        PoloniumRouter(std::string_view default_uri = "") : main_uri(default_uri) {}

        inline void get(const std::string& uri, endpoint_handler handler) { newMethod(http_methods::get, uri, handler); }
        inline void post(const std::string& uri, endpoint_handler handler) { newMethod(http_methods::post, uri, handler); }
        inline void put(const std::string& uri, endpoint_handler handler) { newMethod(http_methods::put, uri, handler); }
        inline void http_delete(const std::string& uri, endpoint_handler handler) { newMethod(http_methods::http_delete, uri, handler); }
        inline void patch(const std::string& uri, endpoint_handler handler) { newMethod(http_methods::patch, uri, handler); }
        inline void head(const std::string& uri, endpoint_handler handler) { newMethod(http_methods::head, uri, handler); }
        inline void options(const std::string& uri, endpoint_handler handler) { newMethod(http_methods::options, uri, handler); }
        inline void connect(const std::string& uri, endpoint_handler handler) { newMethod(http_methods::connect, uri, handler); }
        inline void trace(const std::string& uri, endpoint_handler handler) { newMethod(http_methods::trace, uri, handler); }

        inline void includeDispatcher(Dispatcher& dispatcher) noexcept {
            logger_.trace(__func__);
            for(auto& [method, uri, handler, templates] : routes_)
                dispatcher.registerMethod(std::move(method), std::move(uri), std::move(handler), std::move(templates));
            
            routes_.clear();
            logger_.debug("Methods are included in the dispatcher.");
        }

    private:
        std::vector<Route> routes_;

        inline void newMethod(const char* method, const std::string uri, endpoint_handler handler) {
            logger_.trace(__func__);
            routes_.emplace_back(method, main_uri + uri, handler);
        }
};
