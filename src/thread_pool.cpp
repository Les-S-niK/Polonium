
#include "polonium/thread_pool.hpp"

#include <cstdint>
#include <format>
#include <ranges>
#include <stop_token>
#include <thread>

#include "polonium/polonium_logger.hpp"

ThreadPool::ThreadPool(uint32_t workers_amount) {
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
