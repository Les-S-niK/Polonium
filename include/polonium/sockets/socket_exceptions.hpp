#pragma once

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <unistd.h>

#include <cstdint>
#include <exception>
#include <string>

constexpr inline int8_t status_error = -1;

class socket_exception : public std::exception {
   public:
    socket_exception(std::string message)
        : message("Message: " + std::move(message) + '\n') {}

    socket_exception(std::string message, struct sockaddr_in addr_in)
        : message("Message: " + std::move(message) + '\n' +
                  "Socket addr: " + inet_ntoa(addr_in.sin_addr) + '\n' +
                  "Socket port: " + std::to_string(ntohs(addr_in.sin_port)) +
                  '\n') {}

    [[nodiscard]] auto what() const noexcept -> const char* override {
        return message.c_str();
    }

   private:
    std::string message;
};
