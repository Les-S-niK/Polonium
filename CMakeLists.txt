cmake_minimum_required(VERSION 3.15)
project(polonium CXX)

add_library(
  polonium
  src/sockets/connection_handler.cpp
  src/sockets/tcp_socket.cpp
  src/routing/dispatcher.cpp
  src/polonium_logger.cpp
  src/http/request_parser.cpp
  src/http/response_serializer.cpp
  src/routing/uri_parser.cpp
  src/http/http.cpp
  src/thread_pool.cpp)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(nlohmann_json REQUIRED)
find_package(llhttp REQUIRED)
find_package(ctre REQUIRED)

target_link_libraries(polonium PUBLIC nlohmann_json::nlohmann_json
                                      llhttp::llhttp ctre::ctre)

target_include_directories(
  polonium PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
                  $<INSTALL_INTERFACE:include>)

install(
  TARGETS polonium
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin
  INCLUDES
  DESTINATION include)

install(DIRECTORY include/ DESTINATION include)
