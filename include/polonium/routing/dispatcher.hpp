
#pragma once

#include <functional>
#include <memory>
#include <optional>
#include <string>
#include <unordered_map>

#include "polonium/api_responses.hpp"
#include "polonium/http/http.hpp"
#include "polonium/polonium_logger.hpp"
#include "polonium/routing/uri_parser.hpp"


using endpoint_handler = std::function<std::shared_ptr<ApiResponse> (HttpRequest&& request)>;
using routes_table = std::unordered_map<std::string, std::unordered_map<std::string, std::pair<endpoint_handler, parsed_templates>>>;


struct HandlerWithParams
{
    HandlerWithParams(endpoint_handler handler, std::unordered_map<std::string, UriParamValue> path_params)
     :
       handler(handler), path_params(path_params)
    {}
    HandlerWithParams(std::unordered_map<std::string, UriParamValue> path_params) : path_params(path_params) {}
    HandlerWithParams() {}

    std::optional<endpoint_handler> handler = std::nullopt;
    std::unordered_map<std::string, UriParamValue> path_params;
};


class Dispatcher
{
    public:
        Dispatcher(PoloniumLogger& logger) : logger_(logger) { logger.trace(__func__); }
        ~Dispatcher() { logger_.trace(__func__); }
        void registerMethod(std::string&& method, std::string&& uri, endpoint_handler&& handler, parsed_templates&& templates);
        HandlerWithParams checkRoute(const std::string& method, const std::string& uri) const;
    
    private:
        PoloniumLogger& logger_;
        routes_table routes_; 
};
