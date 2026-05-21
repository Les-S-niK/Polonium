cmake_minimum_required(VERSION 3.15)
project(test_hello)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(polonium REQUIRED)
find_package(glaze REQUIRED)
find_package(llhttp REQUIRED)
find_package(ctre REQUIRED)

add_library(users_router src/users_router.cpp)
target_include_directories(users_router
                           PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(users_router PUBLIC polonium::polonium)

add_executable(hello_app main.cpp)
target_link_libraries(hello_app PUBLIC users_router)

option(REL "Release compiling" OFF)
option(DEBUG "Enable debug build" OFF)
option(UBSAN "UB sanitizer" OFF)
option(MEMSAN "Memory sanitizer" OFF)
option(ASAN "Address sanitizer" ON)
option(THSAN "Thread sanitizer" OFF)

if(REL)
  message("<<< Starting release build >>>")
  target_compile_options(hello_app PRIVATE -g0 -O3 -Wall -Wextra -Wpedantic)
  target_link_options(hello_app PRIVATE -g0 -s)
elseif(DEBUG)
  message("<<< Starting debug build >>>")
  target_compile_options(hello_app PRIVATE -g -O0 -Wall)
elseif(UBSAN)
  message("<<< Starting UB sanitizer build >>> ")
  target_compile_options(
    hello_app
    PRIVATE -g
            -O1
            -fsanitize=address,leak,undefined
            -fno-omit-frame-pointer
            -Wall
            -Wextra
            -Wpedantic)
  target_link_options(hello_app PRIVATE -fsanitize=address,leak,undefined)
elseif(THSAN)
  message("<<< Starting Thread Sanitizer build >>> ")
  target_compile_options(hello_app PRIVATE -O1 -fsanitize=thread,undefined,
                                           -Wall -Wextra -Wpedantic -pthread)
  target_link_options(hello_app PRIVATE -fsanitize=thread,undefined)
elseif(MEMSAN)
  message("<<< Starting MEMSAN sanitizer build >>> ")
  target_compile_options(
    hello_app
    PRIVATE -g
            -O2
            -fsanitize=memory
            -fsanitize-memory-track-origins=2
            -fno-omit-frame-pointer
            -Wall
            -Wextra
            -Wpedantic)
  target_link_options(hello_app PRIVATE -fsanitize=memory
                      -fno-omit-frame-pointer -fsanitize-memory-track-origins=2)
elseif(ASAN)
  message("<<< Starting ASAN sanitizer build >>>")
  target_compile_options(
    hello_app
    PRIVATE -g
            -O2
            -fsanitize=address
            -fno-omit-frame-pointer
            -Wall
            -Wextra
            -Wpedantic)

  target_link_options(hello_app PRIVATE -g -fsanitize=address)
else()
  message("<<< Can't run unspecified build >>>")
endif()
