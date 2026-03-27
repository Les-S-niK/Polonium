
#pragma once

#include <cstdint>
#include <ctime>
#include <filesystem>
#include <fstream>
#include <mutex>
#include <stdexcept>
#include <string>
#include <string_view>

using path = std::filesystem::path;

namespace logger_colors {
inline constexpr std::string_view trace = "\033[0;35m";
inline constexpr std::string_view debug = "\033[0;37;44m";
inline constexpr std::string_view info = "\033[1;37;42m";
inline constexpr std::string_view warning = "\033[1;30;43m";
inline constexpr std::string_view error = "\033[1;37;41m";
inline constexpr std::string_view critical = "\033[4;31;40m";
inline constexpr std::string_view color_default = "\033[0;0;0m";
}  // namespace logger_colors

namespace logger_levels_text {
inline constexpr std::string_view trace = "TRACE: ";
inline constexpr std::string_view debug = "DEBUG: ";
inline constexpr std::string_view info = "INFO: ";
inline constexpr std::string_view warning = "WARNING: ";
inline constexpr std::string_view error = "ERROR: ";
inline constexpr std::string_view critical = "CRITICAL: ";
}  // namespace logger_levels_text

namespace logger_date_formats {
inline constexpr std::string_view filename_format = "%H-%M-%S__%d-%b-%Y";
inline constexpr std::string_view log_str_format = "%H:%M:%S-%d/%m/%Y";
}  // namespace logger_date_formats

enum class LoggerLevels : uint8_t {
    Trace = 0,
    Debug = 1,
    Info = 2,
    Warning = 3,
    Error = 4,
    Critical = 5
};

class PoloniumLogger {
   public:
    static auto getInstance(const path& log_dir, const LoggerLevels& log_level)
        -> PoloniumLogger& {
        static PoloniumLogger logger(log_dir, log_level);
        instance_ptr_ = &logger;
        return logger;
    }

    static auto getInstance() -> PoloniumLogger& {
        if (instance_ptr_ == nullptr) {
            throw std::runtime_error(
                "Logger not initialized. Call getInstance(log_dir, log_level) "
                "first.");
        }
        return *instance_ptr_;
    }

    PoloniumLogger(const PoloniumLogger&) = delete;
    auto operator=(const PoloniumLogger&) -> PoloniumLogger& = delete;

    void trace(std::string_view message) {
        newMessage(message, LoggerLevels::Trace, logger_colors::trace,
                   logger_levels_text::trace);
    }

    void debug(std::string_view message) {
        newMessage(message, LoggerLevels::Debug, logger_colors::debug,
                   logger_levels_text::debug);
    }

    void info(std::string_view message) {
        newMessage(message, LoggerLevels::Info, logger_colors::info,
                   logger_levels_text::info);
    }

    void warning(std::string_view message) {
        newMessage(message, LoggerLevels::Warning, logger_colors::warning,
                   logger_levels_text::warning);
    }

    void error(std::string_view message) {
        newMessage(message, LoggerLevels::Error, logger_colors::error,
                   logger_levels_text::error);
    }

    void critical(std::string_view message) {
        newMessage(message, LoggerLevels::Critical, logger_colors::critical,
                   logger_levels_text::critical);
    }

   private:
    PoloniumLogger(const path& log_dir, const LoggerLevels& log_level);
    ~PoloniumLogger() {
        if (log_file_.is_open()) {
            log_file_.close();
        }
    }

    path log_dir_path_;
    path log_file_path_;
    LoggerLevels log_level_ = LoggerLevels::Debug;
    std::mutex logger_mtx_;
    std::fstream log_file_;
    static PoloniumLogger* instance_ptr_;

    void newMessage(std::string_view message, const LoggerLevels& message_level,
                    std::string_view message_color,
                    std::string_view pre_message_text);
    static auto getCurrentTime(std::string_view format) -> std::string;
};
