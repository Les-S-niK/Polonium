
#include <format>
#include <string_view>
#include <utility>

#include "dispatcher.hpp"
#include "uri_parser.hpp"


void Dispatcher::registerMethod(std::string&& method, std::string&& uri, endpoint_handler&& handler, parsed_templates&& templates) {
    logger_.trace(__func__);

    if (routes_.find(method) == routes_.end())
        routes_.emplace(method, std::unordered_map<std::string, std::pair<endpoint_handler, parsed_templates>>());

    routes_.at(method).emplace(uri, std::make_pair(handler, templates));
    logger_.info(std::format("Registered new method.\nMethod: {}\nUri: {}", method, uri));
}

HandlerWithParams Dispatcher::checkRoute(const std::string& method, const std::string& uri) const {
    logger_.trace(__func__);
    logger_.debug(std::format("Check router.\nMethod: {}\nUri: {}", method, uri));
    
    if(const auto found_method = routes_.find(method); found_method != std::end(routes_)) {
        if(const auto found_uri = found_method->second.find(uri); found_uri != std::end(found_method->second)) {
            // The URI of the method is static and it was found. //
            logger_.debug(std::format("Endpoint was found.\nMethod: {}\nUri: {}", method, uri));
            return HandlerWithParams(found_uri->second.first, {});
        }
        else {
            // The URI of the method is dynamic or it wasn't found. //
            // Need to search in the table of dynamic URIs. //
            logger_.debug("Static endpoint not found. Checking dynamic URIs.");
            for(const auto& method : routes_) {
                for(const auto& uri_template : method.second) {
                    const parsed_templates templates = uri_template.second.second;
                    
                    const auto parsed_values = UriParser(uri, uri_template.first).getUriParamsByTemplate(templates);
                    if(parsed_values.empty()) continue; 

                    logger_.debug("Parsed templates map isn't empty. Trying to get values from the URI.");
                    return HandlerWithParams(uri_template.second.first, parsed_values);
                }
            }
        }
    }
    logger_.debug(std::format("Endpoint not found.\nMethod: {}\nUri: {}", method, uri));
    return HandlerWithParams();

}
