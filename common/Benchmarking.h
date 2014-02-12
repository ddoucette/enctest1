#pragma once
#include <string>
#include <ctime>
#include "object.h"

class BenchmarkObject
{
    public:
        virtual void begin(void)=0;
        virtual void end(void)=0;
        virtual void do_iteration(void)=0;
};

class BenchmarkManager : Object
{
    public:
        BenchmarkManager(std::string name, BenchmarkObject *bobj);
        ~BenchmarkManager();
    
        void set_duration(time_t duration) { this->duration = duration; };
        time_t get_duration(void) { return this->duration; };
        void set_iterations(uint64_t iterations) { this->iterations = iterations; };
        uint64_t get_iterations(void) { return this->iterations; };

        void start(void);
        void run(void);
        void run_duration(time_t duration);
        void run_iterations(uint64_t iterations);
        void stop(void);
        void report(void);

        static void Thread(BenchmarkManager *b);

    private:
        BenchmarkObject *bobj;
        uint64_t    iterations;
        time_t      duration;
};
