
#pragma once

#include <cstdint>
#include <ctime>
#include <filesystem>
#include <fstream>
#include <mutex>
#include <string>
#include <string_view>

namespace polonium {
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
inline constexpr const char* filename_format = "%H-%M-%S__%Y-%m-%d";
inline constexpr const char* log_str_format = "%T-%F";
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
    PoloniumLogger(PoloniumLogger&&) = delete;
    auto operator=(PoloniumLogger&&) -> PoloniumLogger& = delete;
    PoloniumLogger(const PoloniumLogger&) = delete;
    auto operator=(const PoloniumLogger&) -> PoloniumLogger& = delete;

    static auto getInstance(const path& log_dir, const LoggerLevels& log_level)
        -> PoloniumLogger*;
    static auto getInstance() -> PoloniumLogger*;

    auto trace(std::string_view message) -> void;
    auto debug(std::string_view message) -> void;
    auto info(std::string_view message) -> void;
    auto warning(std::string_view message) -> void;
    auto error(std::string_view message) -> void;
    auto critical(std::string_view message) -> void;

   private:
    static inline PoloniumLogger* instance_ptr_ = nullptr;

    PoloniumLogger(const path& log_dir, const LoggerLevels& log_level);
    ~PoloniumLogger();
    void newMessage(std::string_view message, const LoggerLevels& message_level,
                    std::string_view message_color,
                    std::string_view pre_message_text);
    static auto getCurrentTime(const std::string& format) -> std::string;

    path log_dir_path_;
    path log_file_path_;
    LoggerLevels log_level_ = LoggerLevels::Debug;
    std::mutex logger_mtx_;
    std::fstream log_file_;
};
}  // namespace polonium
