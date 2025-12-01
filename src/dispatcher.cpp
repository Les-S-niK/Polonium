
#include <stdexcept>
#include <format>

#include "dispatcher.hpp"
#include "api_responses.hpp"

// TODO: Add path arguments support.

void Dispatcher::registerMethod(std::string&& method, std::string&& uri, std::function<ApiResponse()>&& handler) {
    logger_.trace(__func__);

    if (routes_.find(method) == routes_.end()) {
        routes_.emplace(method, std::unordered_map<std::string, std::function<ApiResponse()>>());
    }
    routes_.at(method).emplace(uri, handler);
    logger_.info(std::format("Registered new method.\nMethod: {}\nUri: {}", method, uri));
}

std::optional<std::function<ApiResponse()>> Dispatcher::checkRoute(const std::string& method, const std::string& uri) {
    logger_.trace(__func__);
    logger_.debug(std::format("Check router.\nMethod: {}\nUri: {}", method, uri));

    try {
        std::function<ApiResponse()> route = routes_.at(method).at(uri);
        logger_.debug(std::format("Endpoint was found.\nMethod: {}\nUri: {}", method, uri));
        return route;
    }
    catch(std::out_of_range& err) {
        logger_.debug(std::format("Endpoint not found.\nMethod: {}\nUri: {}", method, uri));
        return std::nullopt;
    }
}
