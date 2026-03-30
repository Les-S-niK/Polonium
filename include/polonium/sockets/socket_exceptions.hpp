#pragma once

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <array>
#include <cstdint>
#include <exception>
#include <format>
#include <string>
#include <utility>

constexpr inline int8_t status_error = -1;
constexpr inline uint8_t praddr4_len = 4;

class socket_exception : public std::exception {
   public:
    socket_exception(std::string message)
        : message("Message: " + std::move(message) + '\n') {}

    socket_exception(std::string message, struct sockaddr_in addr_in) {
        // TODO: Implement IPv6 support in future.
        std::array<char, praddr4_len> protoaddr{};
        inet_ntop(AF_INET, &addr_in.sin_addr, protoaddr.data(),
                  protoaddr.size());
        this->message = std::format(
            "Message: {}\nSocket addr: {}\nSocket port: {}\n",
            std::move(message), protoaddr.data(), ntohs(addr_in.sin_port));
    }

    [[nodiscard]] auto what() const noexcept -> const char* override {
        return message.c_str();
    }

   private:
    std::string message;
};
