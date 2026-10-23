# Polonium

## Description:

Polonium is a studying-oriented C++ Web HTTP-JSON API Framework.

You can change framework configuration via classes:

```
polonium::PoloniumApiSettings
polonium::PoloniumLoggerSettings
polonium::PoloniumThreadPoolSettings
polonium::PoloinumSocketSettings
```

Polonium allows you to create API routers and separate logic by files.
To create a new router you can inherit `polonium::PoloniumRouter` class and create handlers as
static methods.

Developing finished at the moment.
There will be a better one version of the project named "Plonarium".
( Asynchronous micro-framework with the better performance. )

## Third-party libraries:

JSON parsing and serialization:
`stephenberry/glaze (Copyright (c) 2019 - present, Stephen Berry) MIT`
HTTP parsing:
`nodejs/llhttp (Copyright Fedor Indutny, 2018.) MIT`
Compile-time Reg-ex:
`hanickadot/compile-time-regular-expressions Apache-2.0 license`

---

## Installation:

git pull https://github.com/Les-S-niK/Polonium.git
conan install . --build=missing
conan create .

## Usage

```
#include <cstdint>
#include <polonium/app/polonium.hpp>
#include <polonium/json_parser.hpp>
#include <polonium/sockets/connection_handler.hpp>
#include <utility>

#include "include/users_router.hpp"

auto main() -> int {
    constexpr uint16_t port = 8080;
    constexpr const char* host = "0.0.0.0";

    constexpr uint16_t max_backlog_size = 512;
    constexpr uint8_t timeout_seconds = 10;

    polonium::PoloniumApiSettings api_settings{host, port};
    polonium::PoloniumLoggerSettings logger_settings{
        "./logs", polonium::LoggerLevels::Warning};
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
```

You can check `examples/users_example/` to figure out how to use the framework.

---

## Contributing:

You should not contribute to this project 'cause it's only for studying and finished at the moment.

## License:

MIT License
