
#include "polonium/thread_pool.hpp"

#include <cstdint>
#include <format>
#include <ranges>
#include <stop_token>
#include <thread>
#include <utility>

#include "polonium/app/polonium_logger.hpp"

polonium::ThreadPool::ThreadPool(uint32_t workers_amount) {
    logger_->trace(__func__);
    if (workers_amount == 0) {
        workers_amount = std::jthread::hardware_concurrency();
    }
    logger_->info(std::format("Initialized thread pool with {} threads.",
                              workers_amount));
    workers_.reserve(workers_amount);

    using std::ranges::iota_view;
    for (const auto& index : iota_view(0U, workers_amount)) {
        logger_->debug(std::format("Create thread №{}.", index + 1));
        workers_.emplace_back(
            [this]() -> void { worker_loop(ssource_.get_token()); });
    }
}

polonium::ThreadPool::ThreadPool(ThreadPool&& other) noexcept
    : tasks_(std::move(other.tasks_)),
      logger_(other.logger_),
      workers_(std::move(other.workers_)) {}

auto polonium::ThreadPool::operator=(ThreadPool&& other) noexcept
    -> ThreadPool& {
    if (this != &other) {
        tasks_ = std::move(other.tasks_);
        logger_ = other.logger_;
        workers_ = std::move(other.workers_);
    }
    return *this;
}

auto polonium::ThreadPool::shutdown() -> void {
    logger_->trace(__func__);
    if (ssource_.stop_requested()) {
        return;
    }
    ssource_.request_stop();
    logger_->info("Requested stop for all the threads.");
    tasks_.wakeAllThreads();
}

auto polonium::ThreadPool::worker_loop(const std::stop_token& stoken) -> void {
    while (auto task = tasks_.popFirst(stoken)) {
        task.value()();
    }
}
