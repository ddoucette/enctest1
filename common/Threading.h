#pragma once
#include <memory>
#include <queue>
#include <list>
#include <thread>
#include <condition_variable>
#include "Time.h"

class ThreadPool;
typedef std::shared_ptr<ThreadPool> thread_pool_t;

class ThreadRunnable;
typedef std::shared_ptr<ThreadRunnable> thread_runnable_t;

class ThreadRunnable
{
    friend class ThreadPool;
    public:
        virtual void run(void)=0;
    protected:
        mtime_t last_scheduled;
        uint64_t usec_interval;
};

class ThreadPool : public std::enable_shared_from_this<ThreadPool>
{
    public:
        static thread_pool_t Create(uint32_t pool_size)
        {
            thread_pool_t tpool(new ThreadPool(pool_size));
            tpool->initialize();
            return tpool;
        }

        ~ThreadPool();

        // Schedule the task immediately
        bool schedule(thread_runnable_t task);

        // Schedule the task for the specified interval, in microseconds.
        // Will schedule the task so as to maintain the specified interval
        // between schedules of the specified task.
        bool schedule(thread_runnable_t task, uint32_t usec_interval);

        // Remove from schedule if the thread is periodic.
        void deschedule(thread_runnable_t task);

        // Entry point for each worker thread.
        void thread_entry(void);

    private:
        ThreadPool(uint32_t pool_size);

        // initialize the pool
        void initialize(void);

        // Notify the next waiting worker thread.
        void notify(void);

        // Notify all worker threads.
        void notify_all(void);

        // Task query functions.
        bool task_is_queued(thread_runnable_t task);
        bool task_is_wait_queued(thread_runnable_t task);

        // Scheduled threads
        std::list<thread_runnable_t>    ready_queue;

        // Threads waiting to be scheduled
        std::list<thread_runnable_t>    wait_queue;

        // Flag to indicate if the thread pool is still active.
        bool is_active;

        // Number of threads in the pool
        uint32_t pool_size;

        // Protects queues and state
        std::mutex lock;

        // Condition variable
        std::condition_variable cv;

        // Pool of threads
        std::list<std::thread*> threads;
};
