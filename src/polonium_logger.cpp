
#include <cstring>
#include <iostream>

#include "polonium_logger.hpp"


PoloniumLogger* PoloniumLogger::instance_ptr_ = nullptr;

PoloniumLogger::PoloniumLogger(const path& log_dir, const LoggerLevels& log_level)
 :
  log_dir_path_(log_dir),
  log_file_path_(log_dir_path_ / (getCurrentTime(logger_date_formats::filename_format) + ".plog")),
  log_level_(log_level)
{
    if(!std::filesystem::exists(log_dir))
        std::filesystem::create_directories(log_dir);

    log_file_.open(log_file_path_, std::ios::app);
    if(!log_file_.is_open()) {
        std::cout
            << logger_colors::error << logger_levels_text::error
            << "Can't open log file" << logger_colors::color_default << std::endl;
    }
}


void PoloniumLogger::newMessage(
    std::string_view message,
    const LoggerLevels& message_level,
    std::string_view message_color,
    std::string_view pre_message_text
) {
    if(log_level_ > message_level) return;
    std::unique_lock<std::mutex> lock(logger_mtx_);
    std::string current_time = getCurrentTime(logger_date_formats::log_str_format);
    
    if (message.find('\n') == std::string_view::npos) {
        std::cout
            << message_color << current_time << " "
            << pre_message_text << message
            << logger_colors::color_default << std::endl;
    } 
    else {
        std::cout << message_color << current_time << " " << pre_message_text;
        for(const char& symb : message) {
            if(symb == '\n')
                std::cout << logger_colors::color_default << symb << message_color;
            else 
                std::cout << symb;
        }
        std::cout << logger_colors::color_default << std::endl;
    }
    log_file_ << current_time << " " << pre_message_text << message << std::endl;
}


std::string PoloniumLogger::getCurrentTime(std::string_view format) const {
    constexpr const char* default_time = "00-00-00__00-00-0000";
    constexpr size_t buffer_size = 64;

    std::time_t timestamp = std::time(nullptr);
    if (timestamp == -1) return default_time;

    struct std::tm datetime{};
    if (localtime_r(&timestamp, &datetime) == nullptr) { return default_time; }

    std::string buffer;
    buffer.resize(buffer_size);

    size_t result_size = std::strftime(buffer.data(), buffer_size, format.data(), &datetime);
    if (result_size == 0) return default_time;    
    buffer.resize(result_size);
    
    return buffer;
}

