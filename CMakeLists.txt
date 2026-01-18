cmake_minimum_required(VERSION 3.10)
project(
    Polonium
    VERSION 0.0.6
    LANGUAGES C CXX    
)

set(CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD 20)
set(CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(SRC_PATH ${CMAKE_CURRENT_SOURCE_DIR}/src)

add_library(
    Polonium STATIC
        ${SRC_PATH}/connection_handler.cpp
        ${SRC_PATH}/request_parser.cpp
        ${SRC_PATH}/dispatcher.cpp
        ${SRC_PATH}/polonium_logger.cpp
        ${SRC_PATH}/uri_parser.cpp
)

target_compile_features(Polonium PRIVATE cxx_std_20)
target_include_directories(Polonium PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include/http_parsing
    ${CMAKE_CURRENT_SOURCE_DIR}/include/sockets
    ${CMAKE_CURRENT_SOURCE_DIR}/include/routing
    ${CMAKE_CURRENT_SOURCE_DIR}/third_party/nlohman_json
)

add_library(
    libhttp STATIC
        ${CMAKE_CURRENT_SOURCE_DIR}/third_party/llhttp/llhttp.c
        ${CMAKE_CURRENT_SOURCE_DIR}/third_party/llhttp/http.c
        ${CMAKE_CURRENT_SOURCE_DIR}/third_party/llhttp/api.c
)

target_include_directories(
    libhttp PUBLIC 
        ${CMAKE_CURRENT_SOURCE_DIR}/third_party/llhttp/
        ${CMAKE_CURRENT_SOURCE_DIR}/third_party/ctre/
)

target_link_libraries(
    Polonium PUBLIC
        libhttp
)
