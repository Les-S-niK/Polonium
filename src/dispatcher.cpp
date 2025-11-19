
#include <stdexcept>
#include <format>

#include "dispatcher.hpp"

// TODO: Add path arguments support.

void Dispatcher::registerMethod(std::string&& method, std::string&& uri, std::function<json()>&& handler) {
    logger_.trace(__func__);

    if (routes_.find(method) == routes_.end()) {
        routes_.emplace(method, std::unordered_map<std::string, std::function<json()>>());
    }
    routes_.at(method).emplace(uri, handler);
    logger_.info(std::format("Registered new method.\nMethod: {}\nUri: {}", method, uri));
}

std::optional<std::function<json()>> Dispatcher::checkRoute(const std::string& method, const std::string& uri) {
    logger_.trace(__func__);
    logger_.debug(std::format("Check router.\nMethod: {}\nUri: {}", method, uri));

    try {
        std::function<json()> route = routes_.at(method).at(uri);
        logger_.debug(std::format("Endpoint has found.\nMethod: {}\nUri: {}", method, uri));
        return route;
    }
    catch(std::out_of_range& err) {
        logger_.debug(std::format("Endpoint did not found.\nMethod: {}\nUri: {}", method, uri));
        return std::nullopt;
    }
}
