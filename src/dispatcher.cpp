
#include <stdexcept>

#include "dispatcher.hpp"

// TODO: Add path arguments support.
Dispatcher::Dispatcher() {}

void Dispatcher::register_method(const std::string& method, const std::string& uri, std::function<json()> handler) {
    if (routes_.find(method) == routes_.end()) {
        routes_.emplace(method, std::unordered_map<std::string, std::function<json()>>());
    }
    routes_.at(method).emplace(uri, handler);
}

std::optional<std::function<json()>>
Dispatcher::check_route(const std::string& method, const std::string& uri) {
    try {
        std::function<json()> route = routes_.at(method).at(uri);
        return route;
    }
    catch(std::out_of_range&) {
        return std::nullopt;
    }
}
