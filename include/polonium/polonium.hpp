
#pragma once

#include <string>

#include "polonium/routing/dispatcher.hpp"
#include "polonium/polonium_logger.hpp"
#include "polonium/routing/polonium_router.hpp"
#include "polonium/sockets/connection_handler.hpp"


class App
{
    public:
        App(
            const std::string& host,
            const uint16_t& port,
            std::string_view logs_path,
            const LoggerLevels& log_level
        ):
            logger_(PoloniumLogger::getInstance(logs_path, log_level)),
            dispatcher_(logger_),
            connection_handler_(host, port, logger_, dispatcher_)
        {
            logger_.trace(__func__);
        }
        ~App() { logger_.trace(__func__); }

        void includeRouter(PoloniumRouter* router) {
            logger_.trace(__func__);
            router->includeDispatcher(dispatcher_);
            if(router)
                routers.push_back(router);
        }

        void start() {
            logger_.trace(__func__);
            connection_handler_.acceptConnection();
        }

    private:
        PoloniumLogger& logger_;
        Dispatcher dispatcher_;
        ConnectionHandler connection_handler_;
        std::vector<PoloniumRouter*> routers;
};
