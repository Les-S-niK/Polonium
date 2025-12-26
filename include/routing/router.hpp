
#pragma once

#include <string_view>
#include <utility>

#include "polonium_logger.hpp"
#include "dispatcher.hpp"
#include "uri_parser.hpp"
// TODO: Add the path params support. 
// TODO: Add other http methods.

namespace http_methods {
    constexpr const char* get = "GET"; 
    constexpr const char* post = "POST"; 
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


class Router
{
    public:
        Router(std::string_view default_uri = "") : default_uri_(default_uri) {}

        inline void get(const std::string& uri, endpoint_handler handler) {
            logger_.trace(__func__);
            routes.emplace_back(http_methods::get, default_uri_ + uri, handler);
        }

        inline void post(const std::string& uri, endpoint_handler handler) {
            logger_.trace(__func__);
            routes.emplace_back(http_methods::post, default_uri_ + uri, handler);
        }

        inline void includeDispatcher(Dispatcher& dispatcher) noexcept {
            logger_.trace(__func__);
            for(auto& [method, uri, handler, templates] : routes)
                dispatcher.registerMethod(std::move(method), std::move(uri), std::move(handler), std::move(templates));
            
            routes.clear();
            logger_.debug("Methods are included in the dispatcher.");
        }

    private:
        PoloniumLogger& logger_ = PoloniumLogger::getInstance();
        std::string default_uri_;
        std::vector<Route> routes;
};
