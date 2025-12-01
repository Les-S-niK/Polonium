
#pragma once

#include "dispatcher.hpp"
#include "polonium_logger.hpp"
#include "router.hpp"
#include <string>

#include "connection_handler.hpp"


class App
{
    public:
        App(
            const std::string& host,
            const uint16_t& port,
            std::string_view logs_path,
            const LoggerLevels& log_level
        ) :
            logger_(PoloniumLogger::getInstance(logs_path, log_level)),
            connection_handler_(host, port, logger_, dispatcher_),
            dispatcher_(logger_)
        {
            logger_.trace(__func__);
        }
        ~App() { logger_.trace(__func__); }

        void includeRouter(Router* router) {
            logger_.trace(__func__);
            router->includeDispatcher(dispatcher_);
            if(router) {
                routers.push_back(router);
            }
        }

        void start() {
            logger_.trace(__func__);
            connection_handler_.acceptConnection();
        }

    private:
        PoloniumLogger& logger_;
        Dispatcher dispatcher_;
        ConnectionHandler connection_handler_;
        std::vector<Router*> routers;
};
