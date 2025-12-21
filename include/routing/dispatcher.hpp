
#pragma once

#include <functional>
#include <optional>
#include <string>
#include <unordered_map>

#include "api_responses.hpp"
#include "http/http.hpp"
#include "polonium_logger.hpp"


using endpoint_handler = std::function<ApiResponse(HttpRequest request)>;
using routes_table = std::unordered_map<std::string, std::unordered_map<std::string, endpoint_handler>>;


class Dispatcher
{
    public:
        Dispatcher(PoloniumLogger& logger) : logger_(logger) { logger.trace(__func__); }
        ~Dispatcher() { logger_.trace(__func__); }
        void registerMethod(std::string&& method, std::string&& uri, endpoint_handler&& handler);
        std::optional<endpoint_handler> checkRoute(const std::string& method, const std::string& uri) const;
        uri_path_params findAllPathParams(std::string_view uri) const;
    
    private:
        PoloniumLogger& logger_;
        routes_table routes_; 
};
