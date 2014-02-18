#include "Threading.h"
#include <thread>
#include <mutex>
#include <queue>
#include <list>
#include <condition_variable>

ThreadPool::ThreadPool(uint32_t pool_size)
{
    this->is_active = true;
    {
        std::lock_guard<std::mutex> lck(this->lock);
        for (uint32_t i = 0; i < pool_size; i++)
        {
            std::thread worker(thread_func, this);
            threads.push_front(worker);
        }
    }
}

ThreadPool::~ThreadPool()
{
    // Set is_active to false and notify all worker threads.
    // The worker threads will detect is_active is false
    // and exit.
    // Once the workers have cleared out, we can
    // clean up the runnables.
    {
        std::lock_guard<std::mutex> lck(this->lock);
        this->is_active = false;
    }
    while (!threads.empty())
    {
        std::thread worker = threads.pop_back();
        worker.wait();
    }

    std::lock_guard<std::mutex> lck(this->lock);
    while (!wait_queue.empty())
        wait_queue.pop_back();
    while (!ready_queue.empty())
        ready_queue.pop_back();
}

bool ThreadPool::schedule(thread_runnable_t thread)
{
    // Do not double-queue
    std::lock_guard<std::mutex> lck(this->lock);
    if (find_thread(thread) != NULL)
        return true;
    ready_queue.push(thread);
    return true;
}

void ThreadPool::deschedule(thread_runnable_t thread)
{
    std::lock_guard<std::mutex> lck(this->lock);
    ready_queue.remove(thread);
    wait_queue.remove(thread);
}

void ThreadPool::thread_entry(void)
{
    while (true)
    {
        std::unique_lock<std::mutex> lk(this->lock);
        if (!this->is_active)
            break;

        
        cv.wait(lk);
    }
}

void ThreadPool::notify(void)
{
    this->cv.notify_one();
}
