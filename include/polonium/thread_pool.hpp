/**
 * Studying-only thread pool implementation. Do not use in production.
 */

#pragma once

#include <condition_variable>
#include <cstdint>
#include <deque>
#include <functional>
#include <mutex>
#include <optional>
#include <stop_token>
#include <thread>
#include <vector>

#include "polonium/polonium_logger.hpp"

template <typename F>
concept is_callable = requires(F func) { func(); };

template <typename T>
class ThreadSafeDeque {
   public:
    ThreadSafeDeque() = default;
    ThreadSafeDeque(const ThreadSafeDeque&) = delete;
    ThreadSafeDeque(ThreadSafeDeque&&) = delete;
    auto operator=(const ThreadSafeDeque&) -> ThreadSafeDeque& = delete;
    auto operator=(ThreadSafeDeque&&) -> ThreadSafeDeque& = delete;
    ~ThreadSafeDeque() = default;

    /**
     * Push value of T type to the back of deque.
     *
     * @param value: Value to push back.
     */
    auto pushBack(T value) {
        {
            std::lock_guard<std::mutex> lock(mutex_);
            data_.push_back(std::move(value));
        }
        cv_.notify_one();
    }

    /**
     * Pop the first element from the deque.
     *
     * @param & stoken: stop token from stop source.
     * @return std::optional<T>: nullopt if data is empty. T value if success.
     */
    auto popFirst(const std::stop_token& stoken) -> std::optional<T> {
        std::unique_lock<std::mutex> lock(mutex_);
        cv_.wait(lock, [this, stoken]() -> auto {
            return not data_.empty() or stoken.stop_requested();
        });
        if (data_.empty() and stoken.stop_requested()) {
            return std::nullopt;
        }
        T value = std::move(data_.front());
        data_.pop_front();
        return value;
    }

    /**
     * Wake up all the threads in the ThreadPool.
     */
    void wakeAllThreads() { cv_.notify_all(); }

   private:
    std::mutex mutex_;
    std::deque<T> data_;
    std::condition_variable cv_;
};

class ThreadPool {
   public:
    explicit ThreadPool(
        uint32_t workers_amount = std::jthread::hardware_concurrency());
    ThreadPool(const ThreadPool&) = delete;
    ThreadPool(ThreadPool&&) = delete;
    auto operator=(const ThreadPool&) -> ThreadPool& = delete;
    auto operator=(ThreadPool&&) -> ThreadPool& = delete;
    ~ThreadPool() { shutdown(); };

    /**
     * Add new task ( move_only_function ) in the tasks deque.
     *
     * @param && func: move_only_function to add in the deque.
     */
    template <is_callable F>
    auto addTask(F&& func) -> void {
        logger_->trace(__func__);
        tasks_.pushBack(std::forward<F>(func));
    }

    /**
     * Requests stop for all jthreads and wake up all them.
     */
    void shutdown() {
        logger_->trace(__func__);
        if (ssource_.stop_requested()) {
            return;
        }
        ssource_.request_stop();
        logger_->info("Requested stop for all the threads.");
        tasks_.wakeAllThreads();
    }

   private:
    std::mutex mutex_;
    ThreadSafeDeque<std::move_only_function<void(void)>> tasks_;
    std::stop_source ssource_{};
    PoloniumLogger* logger_ = PoloniumLogger::getInstance();
    std::vector<std::jthread> workers_;

    /**
     * Main worker loop.
     *
     * @param & stoken: stop token got from the stop source.
     */
    void worker_loop(const std::stop_token& stoken) {
        while (auto task = tasks_.popFirst(stoken)) {
            task.value()();
        }
    }
};
