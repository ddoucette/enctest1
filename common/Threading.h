#pragma once
#include <memory>
#include <queue>
#include <list>
#include <thread>
#include <condition_variable>

class ThreadPool;
typedef std::shared_ptr<ThreadPool> thread_pool_t;

class ThreadRunnable;
typedef std::shared_ptr<ThreadRunnable> thread_runnable_t;

class ThreadRunnable
{
    friend ThreadPool;
    public:
        virtual void run(void)=0;
        virtual void get_period(void) { return 0; };
    protected:
        uint64_t last_scheduled;
};

class ThreadPool
{
    public:
        ThreadPool(uint32_t pool_size);
        ~ThreadPool();

        bool schedule(thread_runnable_t thread);

        // Remove from schedule if the thread is periodic.
        void deschedule(thread_runnable_t thread);

        // Entry point for each worker thread.
        void thread_entry(void);

    private:
        // Notify the next waiting worker thread.
        void notify(void);

        // Notify all worker threads.
        void notify_all(void);

        // Scheduled threads
        std::queue<thread_runnable_t>   ready_queue;

        // Threads waiting to be scheduled
        std::queue<thread_runnable_t>   wait_queue;

        // Flag to indicate if the thread pool is still active.
        bool is_active;

        // Protects queues and state
        std::mutex lock;

        // Condition variable
        std::condition_variable cv;

        // Pool of threads
        std::list<std::thread> threads;
};
