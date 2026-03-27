#pragma once

#include <llhttp.h>

#include <exception>
#include <string>

/**
 * @brief llhttp_bad_errno is exception that represents llttp errnos in readable
 * form. You should throw it when llhttp sets bad errno.
 */
class llhttp_bad_errno : public std::exception {
   public:
    llhttp_bad_errno(std::string message, llhttp_errno_t error_num)
        : message("Message is: " + std::move(message) + '\n' + "Errno name: " +
                  std::string(llhttp_errno_name(error_num)) + '\n') {}

    [[nodiscard]] auto what() const noexcept -> const char* override {
        return message.c_str();
    }

   private:
    std::string message;
};
