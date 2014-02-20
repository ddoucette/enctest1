#include "Threading.h"
#include <thread>
#include <mutex>
#include <queue>
#include <list>
#include <condition_variable>
#include "iassert.h"


void thread_func(thread_pool_t thread_pool)
{
    thread_pool->thread_entry();
}

ThreadPool::ThreadPool(uint32_t pool_size)
{
    this->is_active = true;
    {
        std::lock_guard<std::mutex> lck(this->lock);
        for (uint32_t i = 0; i < pool_size; i++)
        {
            std::thread *worker = new std::thread(thread_func, this->shared_from_this());
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
        std::thread *worker = this->threads.front();
        this->threads.pop_front();
        worker->join();
    }

    std::lock_guard<std::mutex> lck(this->lock);
    while (!this->wait_queue.empty())
        this->wait_queue.pop_front();
    while (!this->ready_queue.empty())
        this->ready_queue.pop_front();
}

bool ThreadPool::schedule(thread_runnable_t task)
{
    // Do not double-queue
    std::lock_guard<std::mutex> lck(this->lock);
    if (task_is_queued(task))
        return true;
    ready_queue.push_back(task);
    return true;
}

bool ThreadPool::schedule(thread_runnable_t task, uint32_t usec_interval)
{
    // Do not double-queue
    std::lock_guard<std::mutex> lck(this->lock);
    task->usec_interval = usec_interval;

    if (task_is_queued(task))
        return true;

    ready_queue.push_back(task);
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
    thread_runnable_t task = NULL;
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
                if (task != NULL)
                {
                    // Verify the task can be run.  If so, remove it
                    // from the wait queue and execute.
                }
                else
                {
                    task = ready_queue.front();
                    if (task != NULL)
                        ready_queue.pop_front();
                }

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
    //this->cv.notify_one();
}

bool ThreadPool::task_is_queued(thread_runnable_t task)
{
    for (std::list<thread_runnable_t>::iterator it = wait_queue.begin();
         it != wait_queue.end();
         it++)
    {
        if (*it == task)
            return true;
    }
    for (std::list<thread_runnable_t>::iterator it = ready_queue.begin();
         it != ready_queue.end();
         it++)
    {
        if (*it == task)
            return true;
    }
    return false;
}
