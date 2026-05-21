
#include <cstdint>
#include <polonium/app/polonium.hpp>
#include <polonium/sockets/connection_handler.hpp>
#include <thread>
#include <utility>

#include "include/users_router.hpp"

auto main() -> int {
    constexpr uint16_t port = 8080;
    constexpr const char* host = "0.0.0.0";

    constexpr uint16_t max_backlog_size = 512;
    constexpr uint8_t timeout_seconds = 10;

    polonium::PoloniumApiSettings api_settings{host, port};
    polonium::PoloniumLoggerSettings logger_settings{
        "./logs", polonium::LoggerLevels::Trace};
    polonium::PoloniumThreadPoolSettings thread_pool_settings{};
    polonium::PoloinumSocketSettings socket_settings{max_backlog_size,
                                                     timeout_seconds};

    polonium::PoloniumApp app(
        std::move(api_settings), std::move(socket_settings),
        std::move(logger_settings), std::move(thread_pool_settings));

    users_router::UsersRouter router{};

    app.includeRouter(&router);
    app.start();

    return EXIT_SUCCESS;
}
