#pragma once 

#include <string>
#include <cstdint>
#include <exception>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/types.h>


constexpr inline int8_t status_error = -1;


class socket_exception : public std::exception
{
    public:
        socket_exception(std::string message) :
            message("Message: " + std::move(message) + '\n')
        {}

        socket_exception(std::string message, struct sockaddr_in addr_in) :
            message("Message: " + std::move(message) + '\n' +
                "Socket addr: " + inet_ntoa(addr_in.sin_addr) + '\n' +
                "Socket port: " + std::to_string(ntohs(addr_in.sin_port)) + '\n'
            )
        {}

        const char* what() const noexcept override {
            return message.c_str();
        }

    private:
        std::string message;
};


inline void closeSocketOnError(
    const int& status,
    const int& fd,
    std::string message,
    const struct sockaddr_in& addr_in
) {
    if(status == status_error) {
        close(fd);
        throw socket_exception(std::move(message), addr_in);
    }
}

inline void closeSocketOnError(
    const int& status,
    const int& fd,
    std::string message
) {
    if(status == status_error) {
        close(fd);
        throw socket_exception(std::move(message));
    }
}

inline void closeSocketOnError(const int& status, std::string message) {
    if(status == status_error) {
        throw socket_exception(std::move(message));
    }
}
