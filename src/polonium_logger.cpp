
#include "polonium/polonium_logger.hpp"

#include <cstring>
#include <iostream>
#include <print>

PoloniumLogger::PoloniumLogger(const path& log_dir,
                               const LoggerLevels& log_level)
    : log_dir_path_(log_dir),
      log_file_path_(
          log_dir_path_ /
          (getCurrentTime(logger_date_formats::filename_format) + ".plog")),
      log_level_(log_level) {
    if (!std::filesystem::exists(log_dir)) {
        std::filesystem::create_directories(log_dir);
    }

    log_file_.open(log_file_path_, std::ios::app);
    if (!log_file_.is_open()) {
        std::println("{}{}Can not open log file.{}", logger_colors::error,
                     logger_levels_text::error, logger_colors::color_default);
    }
}

PoloniumLogger::~PoloniumLogger() {
    if (log_file_.is_open()) {
        log_file_.close();
    }
}

auto PoloniumLogger::getInstance(const path& log_dir,
                                 const LoggerLevels& log_level)
    -> PoloniumLogger* {
    static PoloniumLogger logger(log_dir, log_level);
    instance_ptr_ = &logger;
    return &logger;
}

auto PoloniumLogger::getInstance() -> PoloniumLogger* {
    if (instance_ptr_ == nullptr) {
        throw std::runtime_error(
            "Logger not initialized. Call getInstance(log_dir, log_level) "
            "first.");
    }
    return instance_ptr_;
}

auto PoloniumLogger::trace(std::string_view message) -> void {
    newMessage(message, LoggerLevels::Trace, logger_colors::trace,
               logger_levels_text::trace);
}

auto PoloniumLogger::debug(std::string_view message) -> void {
    newMessage(message, LoggerLevels::Debug, logger_colors::debug,
               logger_levels_text::debug);
}

auto PoloniumLogger::info(std::string_view message) -> void {
    newMessage(message, LoggerLevels::Info, logger_colors::info,
               logger_levels_text::info);
}

auto PoloniumLogger::warning(std::string_view message) -> void {
    newMessage(message, LoggerLevels::Warning, logger_colors::warning,
               logger_levels_text::warning);
}

auto PoloniumLogger::error(std::string_view message) -> void {
    newMessage(message, LoggerLevels::Error, logger_colors::error,
               logger_levels_text::error);
}

auto PoloniumLogger::critical(std::string_view message) -> void {
    newMessage(message, LoggerLevels::Critical, logger_colors::critical,
               logger_levels_text::critical);
}

auto PoloniumLogger::newMessage(std::string_view message,
                                const LoggerLevels& message_level,
                                std::string_view message_color,
                                std::string_view pre_message_text) -> void {
    if (log_level_ > message_level) {
        return;
    }
    std::unique_lock<std::mutex> lock(logger_mtx_);
    std::string current_time =
        getCurrentTime(logger_date_formats::log_str_format);

    if (!message.contains('\n')) {
        std::println("{}{} {}{}{}", message_color, current_time,
                     pre_message_text, message, logger_colors::color_default);
    } else {
        std::print("{}{} {}", message_color, current_time, pre_message_text);
        for (const char& symb : message) {
            if (symb == '\n') {
                std::print("{}{}{}", logger_colors::color_default, symb,
                           message_color);
            } else {
                std::print("{}", symb);
            }
        }
        std::println("{}", logger_colors::color_default);
    }
    log_file_ << current_time << " " << pre_message_text << message << '\n';
}

auto PoloniumLogger::getCurrentTime(std::string_view format) -> std::string {
    constexpr const char* default_time = "00-00-00__00-00-0000";
    constexpr size_t buffer_size = 64;

    std::time_t timestamp = std::time(nullptr);
    if (timestamp == -1) {
        return default_time;
    }

    struct std::tm datetime{};
    if (localtime_r(&timestamp, &datetime) == nullptr) {
        return default_time;
    }

    std::string buffer;
    buffer.resize(buffer_size);

    size_t result_size =
        std::strftime(buffer.data(), buffer_size, format.data(), &datetime);
    if (result_size == 0) {
        return default_time;
    }
    buffer.resize(result_size);

    return buffer;
}
