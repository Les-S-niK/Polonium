
#pragma once

#include <ctime>
#include <filesystem>
#include <fstream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <mutex>


using path = std::filesystem::path;

namespace logger_colors {
    inline constexpr std::string_view trace = "\033[0;35m";
    inline constexpr std::string_view debug = "\033[0;37;44m";
    inline constexpr std::string_view info = "\033[1;37;42m";
    inline constexpr std::string_view warning = "\033[1;30;43m";
    inline constexpr std::string_view error = "\033[1;37;41m";
    inline constexpr std::string_view critical = "\033[4;31;40m";
    inline constexpr std::string_view color_default = "\033[0;0;0m";
}

namespace logger_levels_text {
    inline constexpr std::string_view trace = "TRACE: ";
    inline constexpr std::string_view debug = "DEBUG: ";
    inline constexpr std::string_view info = "INFO: ";
    inline constexpr std::string_view warning = "WARNING: ";
    inline constexpr std::string_view error = "ERROR: ";
    inline constexpr std::string_view critical = "CRITICAL: ";
}

namespace logger_date_formats {
    inline constexpr std::string_view filename_format = "%H-%M-%S__%d-%b-%Y";
    inline constexpr std::string_view log_str_format = "%H:%M:%S-%d/%m/%Y";
}


enum class LoggerLevels
{
    Trace = 0,
    Debug = 1,
    Info = 2,
    Warning = 3,
    Error = 4,
    Critical = 5
};


class PoloniumLogger
{
    public:
        inline static PoloniumLogger& getInstance(path log_dir, LoggerLevels log_level) {
            static PoloniumLogger logger(log_dir, log_level);
            instance_ptr_ = &logger;
            return logger;
        }

        inline static PoloniumLogger& getInstance() {
            if(!instance_ptr_) throw std::runtime_error("Logger not initialized. Call getInstance(log_dir, log_level) first.");
            return getInstance("./", LoggerLevels::Info);
        }

        PoloniumLogger(const PoloniumLogger&) = delete;
        PoloniumLogger& operator=(const PoloniumLogger&) = delete; 

        inline void trace(std::string_view message) {
            newMessage(
                message,
                LoggerLevels::Trace,
                logger_colors::trace,
                logger_levels_text::trace
            );
        } 
        
        inline void debug(std::string_view message) {
            newMessage(
                message,
                LoggerLevels::Debug,
                logger_colors::debug,
                logger_levels_text::debug
            );
        }

        inline void info(std::string_view message) {
            newMessage(
                message,
                LoggerLevels::Info,
                logger_colors::info,
                logger_levels_text::info
            );
        }

        inline void warning(std::string_view message) {
            newMessage(
                message,
                LoggerLevels::Warning,
                logger_colors::warning,
                logger_levels_text::warning
            );
        }

        inline void error(std::string_view message) {
            newMessage(
                message,
                LoggerLevels::Error,
                logger_colors::error,
                logger_levels_text::error
            );
        }

        inline void critical(std::string_view message) {
            newMessage(
                message,
                LoggerLevels::Critical,
                logger_colors::critical,
                logger_levels_text::critical
            );
        }

    private:
        PoloniumLogger(path log_dir, LoggerLevels log_level);
        ~PoloniumLogger() { if(log_file_.is_open()) log_file_.close(); }

        std::mutex logger_mtx_;
        std::fstream log_file_;
        path log_dir_path_;
        LoggerLevels log_level_;
        path log_file_path_;  
        static PoloniumLogger* instance_ptr_;

        void newMessage(std::string_view message, LoggerLevels message_level, std::string_view message_color, std::string_view pre_message_text);
        std::string getCurrentTime(std::string_view format) const;
};
