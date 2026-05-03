
#include "polonium/polonium.hpp"

PoloniumApp::PoloniumApp(std::string&& host, uint16_t port,
                         std::string_view logs_path,
                         const LoggerLevels& log_level, uint32_t workers_amount)
    : logger_(PoloniumLogger::getInstance(logs_path, log_level)),
      connection_handler_(std::move(host), port, dispatcher_, workers_amount) {
    logger_->trace(__func__);
}
PoloniumApp::~PoloniumApp() { logger_->trace(__func__); }

auto PoloniumApp::includeRouter(PoloniumRouter* router) -> void {
    logger_->trace(__func__);
    router->includeDispatcher(dispatcher_);
    if (router != nullptr) {
        routers.push_back(router);
    }
}

auto PoloniumApp::start() -> void {
    logger_->trace(__func__);
    connection_handler_.acceptConnection();
}
