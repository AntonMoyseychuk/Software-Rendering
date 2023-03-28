#include "thread_pool.hpp"

namespace util {
    ThreadPool::ThreadPool(std::size_t thread_count) {
        m_threads.reserve(thread_count);
        for (std::size_t i = 0; i < thread_count; ++i) {
            m_threads.emplace_back(&ThreadPool::Run, this);
        }
    }

    ThreadPool::~ThreadPool() {
        m_quit.store(true);

        for (std::size_t i = 0; i < m_threads.size(); ++i) {
            m_tasks_cv.notify_all();
            m_threads[i].join();
        }
    }

    void ThreadPool::Wait(std::int64_t task_id) noexcept {
        std::unique_lock<std::mutex> lock(m_completed_tasks_ids_mutex);
    
        m_completed_tasks_ids_cv.wait(lock, [this, task_id]() -> bool {
            return m_completed_tasks_ids.find(task_id) != m_completed_tasks_ids.end(); 
        });
    }

    void ThreadPool::WaitAll() noexcept {
        std::unique_lock<std::mutex> lock(m_completed_tasks_ids_mutex);
    
        m_completed_tasks_ids_cv.wait(lock, [this]() -> bool {
            std::scoped_lock<std::mutex> task_lock(this->m_tasks_mutex);
            return this->m_tasks.empty() && this->m_last_id == this->m_completed_tasks_ids.size();
        });
    }

    bool ThreadPool::IsCompleted(std::int64_t task_id) noexcept {
        std::scoped_lock<std::mutex> lock(m_completed_tasks_ids_mutex);
        return m_completed_tasks_ids.find(task_id) != m_completed_tasks_ids.cend();
    }

    void ThreadPool::Run() noexcept
    {
        while(!m_quit) {
            std::unique_lock<std::mutex> task_lock(m_tasks_mutex);

            m_tasks_cv.wait(task_lock, [this]() -> bool { 
                return !this->m_tasks.empty() || this->m_quit; 
            });

            if (!m_tasks.empty()) {
                auto elem = std::move(m_tasks.front());
                m_tasks.pop();
                task_lock.unlock();

                elem.first.get();

                std::scoped_lock<std::mutex> completed_task_ids_lock(m_completed_tasks_ids_mutex);
                m_completed_tasks_ids.insert(elem.second);
                m_completed_tasks_ids_cv.notify_all();
            }
        }
    }

    template <typename Func, typename... Args>
    std::int64_t ThreadPool::AddTask(const Func &func, Args &&...args) noexcept {
        auto task_id = m_last_id++;

        std::scoped_lock<std::mutex> tasks_lock(m_tasks_mutex);
        m_tasks.emplace(std::async(std::launch::deferred, func, args...), task_id);

        m_tasks_cv.notify_one();
        return task_id;
    }
}