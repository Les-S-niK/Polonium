
#pragma once

#include <functional>
#include <optional>
#include <string>
#include <unordered_map>

#include "api_responses.hpp"
#include "polonium_logger.hpp"


class Dispatcher
{
    public:
        Dispatcher(PoloniumLogger& logger) : logger_(logger) { logger.trace(__func__); }
        ~Dispatcher() { logger_.trace(__func__); }
        void registerMethod(std::string&& method, std::string&& uri, std::function<ApiResponse()>&& handler);
        std::optional<std::function<ApiResponse()>> checkRoute(const std::string& method, const std::string& uri);
    
    private:
        PoloniumLogger& logger_;
        std::unordered_map<std::string, std::unordered_map<std::string, std::function<ApiResponse()>>> routes_; 
};
