
#pragma once

#include "polonium/app/polonium_config.hpp"
#include "polonium/routing/dispatcher.hpp"
#include "polonium/routing/polonium_router.hpp"
#include "polonium/sockets/connection_handler.hpp"

namespace polonium {
class PoloniumApp {
   public:
    PoloniumApp(PoloniumApiSettings&& api_settings,
                PoloinumSocketSettings&& socket_settings,
                PoloniumLoggerSettings&& logger_settings,
                PoloniumThreadPoolSettings&& thread_pool_settings) noexcept;
    PoloniumApp(const PoloniumApp&) = delete;
    PoloniumApp(PoloniumApp&&) = delete;
    auto operator=(const PoloniumApp&) -> PoloniumApp& = delete;
    auto operator=(PoloniumApp&&) -> PoloniumApp& = delete;
    ~PoloniumApp();

    auto includeRouter(PoloniumRouter* router) -> void;
    auto start() -> void;

   private:
    PoloniumApiSettings api_settings_;
    PoloinumSocketSettings socket_settings_;
    PoloniumLoggerSettings logger_settings_;
    PoloniumThreadPoolSettings thread_pool_settings_;
    PoloniumLogger* logger_;
    Dispatcher dispatcher_;
    ConnectionHandler connection_handler_;
    std::vector<PoloniumRouter*> routers;
};
}  // namespace polonium
