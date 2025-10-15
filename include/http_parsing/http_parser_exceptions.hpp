#pragma once

#include <exception>
#include <string>

#include "../../third_party/llhttp/llhttp.h"


class LlhttpBadErrno : public std::exception
{
    public:
        LlhttpBadErrno(const std::string& message, const llhttp_errno_t error_num) {
            this->message.append(
                "Message is: " + message + '\n' +
                "Errno name: " + std::string(llhttp_errno_name(error_num)) +
                '\n'
            );
        }
        
        const char * what() const noexcept override {
            return message.c_str();
        }

    private:
        std::string message;
};
