
#pragma once

#include <cstdint>
#include <string>
#include <thread>
#include <utility>

#include "polonium/polonium_logger.hpp"
#include "polonium/routing/dispatcher.hpp"
#include "polonium/routing/polonium_router.hpp"
#include "polonium/sockets/connection_handler.hpp"

class PoloniumApp {
   public:
    PoloniumApp(std::string&& host, uint16_t port, std::string_view logs_path,
                const LoggerLevels& log_level,
                uint32_t workers_amount = std::jthread::hardware_concurrency());
    PoloniumApp(const PoloniumApp&) = delete;
    PoloniumApp(PoloniumApp&&) = delete;
    auto operator=(const PoloniumApp&) -> PoloniumApp& = delete;
    auto operator=(PoloniumApp&&) -> PoloniumApp& = delete;
    ~PoloniumApp();

    auto includeRouter(PoloniumRouter* router) -> void;
    auto start() -> void;

   private:
    PoloniumLogger* logger_;
    Dispatcher dispatcher_;
    ConnectionHandler connection_handler_;
    std::vector<PoloniumRouter*> routers;
};
