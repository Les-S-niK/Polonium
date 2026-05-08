
#pragma once

#include <cstdint>
#include <filesystem>
#include <string>
#include <string_view>

#include "polonium/app/polonium_logger.hpp"

namespace polonium {
class PoloniumApiSettings {
   public:
    PoloniumApiSettings(std::string&& host, uint16_t port) noexcept;
    PoloniumApiSettings(const PoloniumApiSettings&) = default;
    PoloniumApiSettings(PoloniumApiSettings&&) = default;
    auto operator=(const PoloniumApiSettings&)
        -> PoloniumApiSettings& = default;
    auto operator=(PoloniumApiSettings&&) -> PoloniumApiSettings& = default;
    ~PoloniumApiSettings() = default;

    [[nodiscard]] auto getHost() const noexcept -> std::string;
    [[nodiscard]] auto getHostView() const noexcept -> std::string_view;
    [[nodiscard]] auto getPort() const noexcept -> uint16_t;

   private:
    std::string host_;
    uint16_t port_{};
};

class PoloniumLoggerSettings {
   public:
    explicit PoloniumLoggerSettings(
        std::string&& logs_path,
        LoggerLevels log_level = LoggerLevels::Info) noexcept;
    PoloniumLoggerSettings(const PoloniumLoggerSettings&) = default;
    PoloniumLoggerSettings(PoloniumLoggerSettings&&) = default;
    auto operator=(const PoloniumLoggerSettings&)
        -> PoloniumLoggerSettings& = default;
    auto operator=(PoloniumLoggerSettings&&)
        -> PoloniumLoggerSettings& = default;
    ~PoloniumLoggerSettings() = default;

    [[nodiscard]] auto getLoggerPath() const noexcept -> std::filesystem::path;
    [[nodiscard]] auto getLoggerStr() const noexcept -> std::string;
    [[nodiscard]] auto getLoggerStrView() const noexcept -> std::string_view;
    [[nodiscard]] auto getLoggerLevel() const noexcept -> LoggerLevels;

   private:
    std::string logs_path_;
    LoggerLevels log_level_;
};

class PoloniumThreadPoolSettings {
   public:
    explicit PoloniumThreadPoolSettings(uint16_t workers_amount) noexcept;
    PoloniumThreadPoolSettings(const PoloniumThreadPoolSettings&) = default;
    PoloniumThreadPoolSettings(PoloniumThreadPoolSettings&&) = default;
    auto operator=(const PoloniumThreadPoolSettings&)
        -> PoloniumThreadPoolSettings& = default;
    auto operator=(PoloniumThreadPoolSettings&&)
        -> PoloniumThreadPoolSettings& = default;
    ~PoloniumThreadPoolSettings() = default;

    [[nodiscard]] auto getWorkersAmount() const noexcept -> uint16_t;

   private:
    uint16_t workers_amount_{};
};

class PoloinumSocketSettings {
   public:
    PoloinumSocketSettings(uint16_t max_backlog_size,
                           size_t timeout_seconds) noexcept;
    PoloinumSocketSettings(const PoloinumSocketSettings&) = default;
    PoloinumSocketSettings(PoloinumSocketSettings&&) = default;
    auto operator=(const PoloinumSocketSettings&)
        -> PoloinumSocketSettings& = default;
    auto operator=(PoloinumSocketSettings&&)
        -> PoloinumSocketSettings& = default;
    ~PoloinumSocketSettings() = default;

    [[nodiscard]] auto getMaxBacklogSize() const noexcept -> uint16_t;
    [[nodiscard]] auto getTimeoutSeconds() const noexcept -> size_t;

   private:
    uint16_t max_backlog_size_;
    size_t timeout_seconds_;
};
}  // namespace polonium
