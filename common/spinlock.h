#pragma once
#include <atomic>
#include <thread>

class Spinlock
{
    public:
        Spinlock() { lval = 0; }
        ~Spinlock() {}

        void lock(void)
        {
            int v = 0;
            while (!lval.compare_exchange_strong(
                            v,
                            1,
                            std::memory_order::memory_order_relaxed,
                            std::memory_order::memory_order_relaxed))
            {
                std::this_thread::yield();
                v = 0;
            }
        }
        void unlock(void) { lval = 0; }

    private:
        std::atomic<int> lval;
};
