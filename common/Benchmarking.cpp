#include <string>
#include <thread>
#include <cassert>

#include "Benchmarking.h"

BenchmarkManager::BenchmarkManager(std::string name, BenchmarkObject *bobj) :
                                   Object(name)
{
    this->iterations = 0;
    this->bobj = bobj;
}

BenchmarkManager::~BenchmarkManager()
{
}

void BenchmarkManager::Thread(BenchmarkManager *b)
{
    b->run();
}

void BenchmarkManager::start(void)
{
    std::thread t(BenchmarkManager::Thread, this);
    t.join();
}

void BenchmarkManager::stop(void)
{
}

void BenchmarkManager::run(void)
{
    if (this->iterations > 0)
        this->run_iterations(this->iterations);
    else if (this->duration > 0)
        this->run_duration(this->duration);
    else
        assert(false);
}

void BenchmarkManager::run_duration(time_t duration)
{
    assert(this->bobj != NULL);
    assert(duration > 0);

    time_t start_time = std::time(NULL);
    time_t end_time = start_time + duration;
    time_t current_time = start_time;
    this->iterations = 0;
    this->bobj->begin();
    while (current_time < end_time)
    {
        this->bobj->do_iteration();
        current_time = std::time(NULL);
        this->iterations++;
    }
    this->duration = current_time - start_time;
    this->bobj->end();
}

void BenchmarkManager::run_iterations(uint64_t iterations)
{
    assert(this->bobj != NULL);
    assert(iterations > 0);

    this->iterations = 0;
    time_t start_time = std::time(NULL);
    this->bobj->begin();
    while (this->iterations != iterations)
    {
        this->bobj->do_iteration();
        this->iterations++;
    }
    time_t end_time = std::time(NULL);
    this->duration = end_time - start_time;
    this->bobj->end();
}
