
#pragma once

#include <concepts>
#include <functional>
#include <memory>
#include <optional>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <utility>

#include "polonium/app/polonium_logger.hpp"
#include "polonium/http/http.hpp"
#include "polonium/routing/api_responses.hpp"
#include "polonium/routing/uri_parser.hpp"

namespace polonium {
template <typename F, typename Ret, typename... Args>
concept is_endpoint_handler =
    std::invocable<F, HttpRequest&&> and
    std::same_as<Ret, std::invoke_result_t<F, Args...>>;

using endpoint_handler =
    std::function<std::shared_ptr<ApiResponse>(HttpRequest&&)>;
using routes_table = std::unordered_map<
    std::string, std::unordered_map<std::string, std::pair<endpoint_handler,
                                                           parsed_templates>>>;

template <is_endpoint_handler<std::shared_ptr<ApiResponse>, HttpRequest&&> F>
struct HandlerWithParams {
    HandlerWithParams(
        const F& handler,
        std::unordered_map<std::string, UriParamValue> path_params)
        : handler(handler), path_params(std::move(path_params)) {}
    explicit HandlerWithParams(
        std::unordered_map<std::string, UriParamValue> path_params)
        : path_params(std::move(path_params)) {}
    HandlerWithParams() = default;

    std::optional<endpoint_handler> handler;
    std::unordered_map<std::string, UriParamValue> path_params;
};

class Dispatcher {
   public:
    Dispatcher(const Dispatcher&) = default;
    Dispatcher(Dispatcher&&) = delete;
    auto operator=(const Dispatcher&) -> Dispatcher& = delete;
    auto operator=(Dispatcher&&) -> Dispatcher& = delete;
    explicit Dispatcher();
    ~Dispatcher();

    auto registerMethod(std::string&& method, std::string&& uri,
                        const endpoint_handler& handler,
                        parsed_templates&& templates) -> void;
    auto checkRoute(const std::string& method, const std::string& uri)
        -> HandlerWithParams<endpoint_handler>;

   private:
    polonium::PoloniumLogger* logger_;
    routes_table routes_;
};
}  // namespace polonium
