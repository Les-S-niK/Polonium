
#include "polonium/routing/dispatcher.hpp"

#include <format>
#include <ranges>
#include <string_view>
#include <utility>

#include "polonium/routing/uri_parser.hpp"

polonium::Dispatcher::Dispatcher()
    : logger_(polonium::PoloniumLogger::getInstance()) {
    logger_->trace(__func__);
}
polonium::Dispatcher::~Dispatcher() { logger_->trace(__func__); }

auto polonium::Dispatcher::registerMethod(std::string&& method,
                                          std::string&& uri,
                                          const endpoint_handler& handler,
                                          parsed_templates&& templates)
    -> void {
    logger_->trace(__func__);

    if (!routes_.contains(method)) {
        routes_.emplace(
            method,
            std::vector<std::pair<
                std::string, std::pair<endpoint_handler, parsed_templates>>>());
    }

    routes_.at(method).emplace_back(
        uri, std::make_pair(handler, std::move(templates)));
    logger_->info(std::format("Registered new method.\nMethod: {}\nUri: {}",
                              std::move(method), std::move(uri)));
}

auto polonium::Dispatcher::checkRoute(const std::string& method,
                                      const std::string& uri)
    -> HandlerWithParams<endpoint_handler> {
    logger_->trace(__func__);
    logger_->info(
        std::format("Check router.\nMethod: {}\nUri: {}", method, uri));
    if (not validateUri(uri)) {
        return {};
    }

    if (const auto found_method = routes_.find(method);
        found_method != std::end(routes_)) {
        using std::ranges::views::enumerate;
        for (const auto& [index, value] : enumerate(found_method->second)) {
            if (value.first == uri) {
                logger_->debug(std::format(
                    "Endpoint was found.\nMethod: {}\nUri: {}", method, uri));
                return {value.second.first, {}};
            }
        }
        // The URI of the method is dynamic or it wasn't found. //
        // Need to search in the table of dynamic URIs. //
        logger_->debug("Static endpoint not found. Checking dynamic URIs.");

        for (const auto& method : routes_) {
            for (const auto& uri_template : method.second) {
                const parsed_templates templates = uri_template.second.second;
                const auto parsed_values =
                    UriParser(uri, uri_template.first)
                        .getUriParamsByTemplate(templates);
                if (parsed_values.empty()) {
                    continue;
                }

                logger_->debug(
                    "Parsed templates map isn't empty. Trying to get "
                    "values from the URI.");
                return {uri_template.second.first, parsed_values};
            }
        }
    }
    logger_->debug(
        std::format("Endpoint not found.\nMethod: {}\nUri: {}", method, uri));
    return {};
}
auto polonium::Dispatcher::validateUri(std::string_view uri) noexcept -> bool {
    return uri.back() == '/';
}
