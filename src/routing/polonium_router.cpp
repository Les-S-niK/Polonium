
#include "polonium/routing/polonium_router.hpp"

#include <utility>

#include "polonium/polonium_logger.hpp"

Route::Route(std::string method, std::string uri, endpoint_handler handler)
    : method(std::move(method)),
      uri(std::move(uri)),
      handler(std::move(handler)) {
    templates = UriTemplateParser(this->uri).getUriParamsTemplate();
}

PoloniumRouter::PoloniumRouter(std::string default_uri)
    : default_uri_(std::move(default_uri)),
      logger_(PoloniumLogger::getInstance()) {}

[[nodiscard]] auto PoloniumRouter::getDefaultUri() const noexcept
    -> std::string {
    return default_uri_;
}
[[nodiscard]] auto PoloniumRouter::getDefaultUriView() const noexcept
    -> std::string_view {
    return default_uri_;
}
auto PoloniumRouter::setDefaultUri(std::string value) noexcept
    -> PoloniumRouter& {
    default_uri_ = std::move(value);
    return *this;
}

auto PoloniumRouter::includeDispatcher(Dispatcher& dispatcher) noexcept
    -> void {
    logger_->trace(__func__);
    for (auto& [method, uri, handler, templates] : routes_) {
        dispatcher.registerMethod(std::move(method), std::move(uri), handler,
                                  std::move(templates));
    }
    routes_.clear();
    logger_->debug("Methods are included in the dispatcher.");
}

auto PoloniumRouter::get(const std::string& uri, endpoint_handler handler)
    -> void {
    newMethod(http_methods::get, uri, std::move(handler));
}
auto PoloniumRouter::post(const std::string& uri, endpoint_handler handler)
    -> void {
    newMethod(http_methods::post, uri, std::move(handler));
}
auto PoloniumRouter::put(const std::string& uri, endpoint_handler handler)
    -> void {
    newMethod(http_methods::put, uri, std::move(handler));
}
auto PoloniumRouter::deleteHttp(const std::string& uri,
                                endpoint_handler handler) -> void {
    newMethod(http_methods::http_delete, uri, std::move(handler));
}
auto PoloniumRouter::patch(const std::string& uri, endpoint_handler handler)
    -> void {
    newMethod(http_methods::patch, uri, std::move(handler));
}
auto PoloniumRouter::head(const std::string& uri, endpoint_handler handler)
    -> void {
    newMethod(http_methods::head, uri, std::move(handler));
}
auto PoloniumRouter::options(const std::string& uri, endpoint_handler handler)
    -> void {
    newMethod(http_methods::options, uri, std::move(handler));
}
auto PoloniumRouter::connect(const std::string& uri, endpoint_handler handler)
    -> void {
    newMethod(http_methods::connect, uri, std::move(handler));
}
auto PoloniumRouter::trace(const std::string& uri, endpoint_handler handler)
    -> void {
    newMethod(http_methods::trace, uri, std::move(handler));
}
auto PoloniumRouter::newMethod(const char* method, const std::string& uri,
                               endpoint_handler handler) -> void {
    logger_->trace(__func__);
    routes_.emplace_back(method, getDefaultUri() + uri, std::move(handler));
}
