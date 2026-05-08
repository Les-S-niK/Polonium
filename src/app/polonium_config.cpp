
#include "polonium/app/polonium_config.hpp"

polonium::PoloniumApiSettings::PoloniumApiSettings(std::string&& host,
                                                   uint16_t port) noexcept
    : host_(std::move(host)), port_(port) {}
[[nodiscard]] auto polonium::PoloniumApiSettings::getHost() const noexcept
    -> std::string {
    return host_;
}
[[nodiscard]] auto polonium::PoloniumApiSettings::getHostView() const noexcept
    -> std::string_view {
    return host_;
}
[[nodiscard]] auto polonium::PoloniumApiSettings::getPort() const noexcept
    -> uint16_t {
    return port_;
}

polonium::PoloniumLoggerSettings::PoloniumLoggerSettings(
    std::string&& logs_path, LoggerLevels log_level) noexcept
    : logs_path_(std::move(logs_path)), log_level_(log_level) {}

[[nodiscard]] auto polonium::PoloniumLoggerSettings::getLoggerPath()
    const noexcept -> std::filesystem::path {
    return logs_path_;
}

[[nodiscard]] auto polonium::PoloniumLoggerSettings::getLoggerStr()
    const noexcept -> std::string {
    return logs_path_;
}

[[nodiscard]] auto polonium::PoloniumLoggerSettings::getLoggerStrView()
    const noexcept -> std::string_view {
    return logs_path_;
}

[[nodiscard]] auto polonium::PoloniumLoggerSettings::getLoggerLevel()
    const noexcept -> LoggerLevels {
    return log_level_;
}

polonium::PoloniumThreadPoolSettings::PoloniumThreadPoolSettings(
    uint16_t workers_amount) noexcept
    : workers_amount_(workers_amount) {}

[[nodiscard]] auto polonium::PoloniumThreadPoolSettings::getWorkersAmount()
    const noexcept -> uint16_t {
    return workers_amount_;
}
polonium::PoloinumSocketSettings::PoloinumSocketSettings(
    uint16_t max_backlog_size, size_t timeout_seconds) noexcept
    : max_backlog_size_(max_backlog_size), timeout_seconds_(timeout_seconds) {}

[[nodiscard]] auto polonium::PoloinumSocketSettings::getMaxBacklogSize()
    const noexcept -> uint16_t {
    return max_backlog_size_;
}

[[nodiscard]] auto polonium::PoloinumSocketSettings::getTimeoutSeconds()
    const noexcept -> size_t {
    return timeout_seconds_;
}
