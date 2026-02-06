cmake_minimum_required(VERSION 3.15)
project(polonium CXX)

add_library(polonium
    src/connection_handler.cpp
    src/dispatcher.cpp
    src/polonium_logger.cpp
    src/request_parser.cpp
    src/uri_parser.cpp
)

find_package(nlohmann_json REQUIRED)
find_package(llhttp REQUIRED)
find_package(ctre REQUIRED)

target_link_libraries(polonium PUBLIC
    nlohmann_json::nlohmann_json
    llhttp::llhttp
    ctre::ctre
)

target_include_directories(polonium PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

install(TARGETS polonium
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION include
)

install(DIRECTORY include/
    DESTINATION include
)
