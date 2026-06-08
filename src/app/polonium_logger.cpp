
#include "polonium/app/polonium_logger.hpp"

#include <chrono>
#include <format>
#include <iostream>
#include <print>

polonium::PoloniumLogger::PoloniumLogger(const path& log_dir,
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

polonium::PoloniumLogger::~PoloniumLogger() {
    if (log_file_.is_open()) {
        log_file_.close();
    }
}

auto polonium::PoloniumLogger::getInstance(const path& log_dir,
                                           const LoggerLevels& log_level)
    -> polonium::PoloniumLogger* {
    static polonium::PoloniumLogger logger(log_dir, log_level);
    instance_ptr_ = &logger;
    return &logger;
}

auto polonium::PoloniumLogger::getInstance() -> PoloniumLogger* {
    if (instance_ptr_ == nullptr) {
        throw std::runtime_error(
            "Logger not initialized. Call getInstance(log_dir, log_level) "
            "first.");
    }
    return instance_ptr_;
}

auto polonium::PoloniumLogger::trace(std::string_view message) -> void {
    newMessage(message, LoggerLevels::Trace, logger_colors::trace,
               logger_levels_text::trace);
}

auto polonium::PoloniumLogger::debug(std::string_view message) -> void {
    newMessage(message, LoggerLevels::Debug, logger_colors::debug,
               logger_levels_text::debug);
}

auto polonium::PoloniumLogger::info(std::string_view message) -> void {
    newMessage(message, LoggerLevels::Info, logger_colors::info,
               logger_levels_text::info);
}

auto polonium::PoloniumLogger::warning(std::string_view message) -> void {
    newMessage(message, LoggerLevels::Warning, logger_colors::warning,
               logger_levels_text::warning);
}

auto polonium::PoloniumLogger::error(std::string_view message) -> void {
    newMessage(message, LoggerLevels::Error, logger_colors::error,
               logger_levels_text::error);
}

auto polonium::PoloniumLogger::critical(std::string_view message) -> void {
    newMessage(message, LoggerLevels::Critical, logger_colors::critical,
               logger_levels_text::critical);
}

auto polonium::PoloniumLogger::newMessage(std::string_view message,
                                          const LoggerLevels& message_level,
                                          std::string_view message_color,
                                          std::string_view pre_message_text)
    -> void {
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

auto polonium::PoloniumLogger::getCurrentTime(const std::string& format)
    -> std::string {
    using std::chrono::system_clock;

    std::chrono::time_point now_time = system_clock::now();
    return std::vformat(std::format("{{}}", format),
                        std::make_format_args(now_time));
}
