
#include <stdexcept>
#include <format>

#include "dispatcher.hpp"

// TODO: Add path arguments support.

void Dispatcher::register_method(const std::string& method, const std::string& uri, std::function<json()> handler) {
    logger_.trace(__func__);
    if (routes_.find(method) == routes_.end()) {
        routes_.emplace(method, std::unordered_map<std::string, std::function<json()>>());
    }
    routes_.at(method).emplace(uri, handler);
    logger_.debug(std::format("Registered new method.\nMethod: {}\nUri: {}", method, uri));
}

std::optional<std::function<json()>> Dispatcher::check_route(const std::string& method, const std::string& uri) {
    logger_.trace(__func__);
    logger_.debug(std::format("Check router.\nMethod: {}\nUri: {} ", method, uri));
    try {
        std::function<json()> route = routes_.at(method).at(uri);
        return route;
    }
    catch(std::out_of_range&) {
        return std::nullopt;
    }
}
