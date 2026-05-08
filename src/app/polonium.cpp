
#include "polonium/app/polonium.hpp"

polonium::PoloniumApp::PoloniumApp(
    polonium::PoloniumApiSettings&& api_settings,
    polonium::PoloinumSocketSettings&& socket_settings,
    polonium::PoloniumLoggerSettings&& logger_settings,
    polonium::PoloniumThreadPoolSettings&& thread_pool_settings) noexcept
    : api_settings_(std::move(api_settings)),
      socket_settings_(std::move(socket_settings)),
      logger_settings_(std::move(logger_settings)),
      thread_pool_settings_(std::move(thread_pool_settings)),
      logger_(PoloniumLogger::getInstance(logger_settings_.getLoggerPath(),
                                          logger_settings_.getLoggerLevel())),
      connection_handler_(api_settings_, socket_settings_,
                          thread_pool_settings_, dispatcher_) {
    logger_->trace(__func__);
}
polonium::PoloniumApp::~PoloniumApp() { logger_->trace(__func__); }

auto polonium::PoloniumApp::includeRouter(PoloniumRouter* router) -> void {
    logger_->trace(__func__);
    router->includeDispatcher(dispatcher_);
    if (router != nullptr) {
        routers.push_back(router);
    }
}

auto polonium::PoloniumApp::start() -> void {
    logger_->trace(__func__);
    connection_handler_.acceptConnection();
}
