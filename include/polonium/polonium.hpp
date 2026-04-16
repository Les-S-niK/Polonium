
#pragma once

#include <cstdint>
#include <string>
#include <thread>
#include <utility>

#include "polonium/polonium_logger.hpp"
#include "polonium/routing/dispatcher.hpp"
#include "polonium/routing/polonium_router.hpp"
#include "polonium/sockets/connection_handler.hpp"

class App {
   public:
    App(const App&) = delete;
    App(App&&) = delete;
    auto operator=(const App&) -> App& = delete;
    auto operator=(App&&) -> App& = delete;
    App(std::string&& host, uint16_t port, std::string_view logs_path,
        const LoggerLevels& log_level,
        uint32_t workers_amount = std::jthread::hardware_concurrency())
        : logger_(PoloniumLogger::getInstance(logs_path, log_level)),
          connection_handler_(std::move(host), port, dispatcher_,
                              workers_amount) {
        logger_->trace(__func__);
    }
    ~App() { logger_->trace(__func__); }

    void includeRouter(PoloniumRouter* router) {
        logger_->trace(__func__);
        router->includeDispatcher(dispatcher_);
        if (router != nullptr) {
            routers.push_back(router);
        }
    }

    void start() {
        logger_->trace(__func__);
        connection_handler_.acceptConnection();
    }

   private:
    PoloniumLogger* logger_;
    Dispatcher dispatcher_;
    ConnectionHandler connection_handler_;
    std::vector<PoloniumRouter*> routers;
};
