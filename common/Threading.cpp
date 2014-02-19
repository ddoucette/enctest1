#include "Threading.h"
#include <thread>
#include <mutex>
#include <queue>
#include <list>
#include <condition_variable>

void thread_func(void *arg)
{
    thread_pool_t thread_pool = static_cast<thread_pool_t>(arg);
    thread_pool->thread_entry();
}

ThreadPool::ThreadPool(uint32_t pool_size)
{
    this->is_active = true;
    {
        std::lock_guard<std::mutex> lck(this->lock);
        for (uint32_t i = 0; i < pool_size; i++)
        {
            std::thread worker(thread_func, this->shared_from_this());
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
        wait_queue.pop();
    while (!ready_queue.empty())
        ready_queue.pop();
}

bool ThreadPool::schedule(thread_runnable_t task)
{
    // Do not double-queue
    std::lock_guard<std::mutex> lck(this->lock);
    if (find_thread(task) != NULL)
        return true;
    ready_queue.push(task);
    return true;
}

bool ThreadPool::schedule(thread_runnable_t task, uint32_t usec_interval)
{
    // Do not double-queue
    std::lock_guard<std::mutex> lck(this->lock);
    task->usec_interval = usec_interval;

    if (find_thread(task) != NULL)
        return true;

    ready_queue.push(task);
    return true;
}

void ThreadPool::deschedule(thread_runnable_t task)
{
    std::lock_guard<std::mutex> lck(this->lock);
    ready_queue.remove(task);
    wait_queue.remove(task);
}

void ThreadPool::thread_entry(void)
{
    thread_runnable_t task = NULL
    while (true)
    {
        {
            std::unique_lock<std::mutex> lk(this->lock);
            if (!this->is_active)
                break;
            do
            {
                // First, pick a task from the wait queue.  If not,
                // pick from the ready queue.
                task = wait_queue.front();
                if (task == NULL)
                    task = ready_queue.pop_back();

                if (task == NULL)
                    cv.wait(lk);
            } while (task == NULL);
        }
        // We are now outside of the lock, and we should have
        // a task to run now.
        mASSERT_DEBUG(task != NULL);
        task->run();
        task = NULL;
    }
}

void ThreadPool::notify(void)
{
    this->cv.notify_one();
}
