#pragma once
#include <queue>
#include <vector>
#include <unordered_set>

#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <atomic>

namespace util {
    class ThreadPool final {
    public:
        ThreadPool(std::size_t thread_count);
        ~ThreadPool();

        template <typename Func, typename ...Args>
        std::int64_t AddTask(const Func& func, Args&&... args) noexcept {
            auto task_id = m_last_id++;

            std::scoped_lock<std::mutex> tasks_lock(m_tasks_mutex);
            m_tasks.emplace(std::async(std::launch::deferred, func, args...), task_id);

            m_tasks_cv.notify_one();
            return task_id;
        }

        void Wait(std::int64_t task_id) const noexcept;
        void WaitAll() const noexcept;

        bool IsCompleted(std::int64_t task_id) const noexcept;

    protected:
        void Run() noexcept;

    protected:
        std::vector<std::thread> m_threads;

        std::queue<std::pair<std::future<void>, std::int64_t>> m_tasks;
        mutable std::mutex m_tasks_mutex;
        mutable std::condition_variable m_tasks_cv;

        std::unordered_set<std::int64_t> m_completed_tasks_ids;
        mutable std::mutex m_completed_tasks_ids_mutex;
        mutable std::condition_variable m_completed_tasks_ids_cv;

        std::atomic<bool> m_quit = false;
        std::atomic<int64_t> m_last_id = 0; // variable that handles an id which will be assigned to the next task
    };
}