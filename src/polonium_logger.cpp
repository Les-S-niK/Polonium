
#include <iostream>

#include "polonium_logger.hpp"


PoloniumLogger* PoloniumLogger::instance_ptr_ = nullptr;

PoloniumLogger::PoloniumLogger(path log_dir, LoggerLevels log_level) :
    log_dir_path_(log_dir),
    log_level_(log_level)
{
    if(!std::filesystem::exists(log_dir)) {
        std::filesystem::create_directories(log_dir);
    }
    std::string filename = getCurrentTime(logger_date_formats::filename_format) + ".plog";
    log_file_path_ = log_dir_path_ / filename;

    log_file_.open(log_file_path_, std::ios::app);
    if(!log_file_.is_open()) {
        std::cout
            << logger_colors::error << logger_levels_text::error
            << "Can't open log file" << logger_colors::color_default << std::endl;
    }
}


void PoloniumLogger::newMessage(
    std::string_view message,
    LoggerLevels message_level,
    std::string_view message_color,
    std::string_view pre_message_text
) {
    if(log_level_ > message_level) return;
    std::unique_lock<std::mutex> lock(logger_mtx_);

    std::string current_time = getCurrentTime(logger_date_formats::log_str_format);
    std::cout
        << message_color << current_time << " "
        << pre_message_text << message
        << logger_colors::color_default << std::endl;
    
    log_file_
        << current_time << " " << pre_message_text
        << message << std::endl;
}


std::string PoloniumLogger::getCurrentTime(std::string_view format) const {
    time_t timestamp = time(NULL);
    struct tm datetime = *localtime(&timestamp);

    std::stringstream string_stream;
    string_stream << std::put_time(&datetime, format.data());
    return string_stream.str();
}

