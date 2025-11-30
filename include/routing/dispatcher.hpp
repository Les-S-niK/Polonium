
#pragma once

#include <functional>
#include <optional>
#include <string>
#include <unordered_map>

#include "http/http.hpp" 
#include "polonium_logger.hpp"


class Dispatcher
{
    public:
        Dispatcher(PoloniumLogger& logger) : logger_(logger) { logger.trace(__func__); }
        ~Dispatcher() { logger_.trace(__func__); }
        void registerMethod(std::string&& method, std::string&& uri, std::function<json()>&& handler);
        std::optional<std::function<json()>> checkRoute(const std::string& method, const std::string& uri);
    
    private:
        PoloniumLogger& logger_;
        std::unordered_map<std::string, std::unordered_map<std::string, std::function<json()>>> routes_; 
};
