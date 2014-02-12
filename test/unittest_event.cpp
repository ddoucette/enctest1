#include <iostream>
#include <cassert>
#include <memory>
#include <string>
#include <thread>
#include "object.h"
#include "logger.h"
#include "Events.h"
#include "Benchmarking.h"
#include "pstack.h"
#include "ptrstack.h"
#include "ptrqueue.h"
#include "pqueue.h"
#include "spinlock.h"

enum test_events
{
    TEST_EVENT_1 = 0x14556,
    TEST_EVENT_2,
    TEST_EVENT_3,
    TEST_EVENT_4,
};


class TestEventReceiver;

class TestEventSource :
        public Object,
        public EventSource
{
    public:
        TestEventSource(std::string name) : Object(name) {};
        ~TestEventSource() {};
};

class TestEventReceiver :
        public Object,
        public EventReceiver
{
    public:
        TestEventReceiver(std::string name) : Object(name) {};
        ~TestEventReceiver() {};
void event_rcv(std::shared_ptr<EventSource> src, event_t event)
        {
            events.push_back(event);
        }
        std::list<event_t> events;
};

class EventBenchmark : public BenchmarkObject, public Object
{
    public:
        EventBenchmark(std::string name) : Object(name) {};
        ~EventBenchmark() {};
        void begin(void);
        void end(void);
        void do_iteration(void);

    private:
        std::shared_ptr<TestEventSource> event_source;
        std::shared_ptr<TestEventReceiver> event_receiver;
};

void EventBenchmark::begin(void)
{
    event_source = std::make_shared<TestEventSource>("event1");
    event_receiver = std::make_shared<TestEventReceiver>("receiver1");
    event_source->register_event_receiver(event_receiver);
}

void EventBenchmark::end(void)
{
}

void EventBenchmark::do_iteration(void)
{
    event_source->trigger_event(TEST_EVENT_1);
    event_receiver->events.pop_front();
}

void test1(void)
{
    //
    // 4 Test event sources, 4 test event receivers.
    // Verify:
    // 1) 1-EventSource-to-1-EventReceiver
    // 2) N-EventSource-to-1-EventReceiver
    // 3) 1-EventSource-to-N-EventReceiver
    auto t1 = std::make_shared<TestEventSource>("t1");
    auto t2 = std::make_shared<TestEventSource>("t2");
    auto t3 = std::make_shared<TestEventSource>("t3");
    auto t4 = std::make_shared<TestEventSource>("t4");
    auto r1 = std::make_shared<TestEventReceiver>("r1");
    auto r2 = std::make_shared<TestEventReceiver>("r2");
    auto r3 = std::make_shared<TestEventReceiver>("r3");
    auto r4 = std::make_shared<TestEventReceiver>("r4");

    t1->register_event_receiver(r1);
    t1->trigger_event(TEST_EVENT_1);
    assert(r1->events.size() == 1);
    assert(r1->events.front() == TEST_EVENT_1);
    r1->events.pop_front();

    t2->register_event_receiver(r2);
    t3->register_event_receiver(r2);
    t4->register_event_receiver(r2);
    t2->trigger_event(TEST_EVENT_2);
    t3->trigger_event(TEST_EVENT_3);
    t4->trigger_event(TEST_EVENT_4);
    assert(r2->events.size() == 3);
    assert(r2->events.front() == TEST_EVENT_2);
    r2->events.pop_front();
    assert(r2->events.front() == TEST_EVENT_3);
    r2->events.pop_front();
    assert(r2->events.front() == TEST_EVENT_4);
    std::cout << "test1() PASSED!\n";
}

void test2(void)
{
    EventBenchmark *bmark = new EventBenchmark("test2");
    BenchmarkManager *bmgr = new BenchmarkManager("test2-manager", bmark);
    bmgr->set_duration((time_t)10);
    bmgr->start();
    std::cout << "test1() PASSED!\n";
    uint64_t iterations = bmgr->get_iterations();
    time_t duration = bmgr->get_duration();
    double ops_per_sec = (double)iterations / (double)duration;
    std::cout << " - iterations (" << iterations << ")\n";
    std::cout << " - duration (" << duration << ")\n";
    std::cout << " - ops/sec (" << ops_per_sec << ")\n";
    std::cout << "test2() PASSED!\n";
}

class TestObject1
{
    public:
        TestObject1(uint32_t idx) { this->idx = idx;};
        ~TestObject1() {};
        uint32_t get_idx(void) { return this->idx; };
    private:
        uint32_t idx;
};

class TestObject2 : public ReferencedObject
{
    public:
        TestObject2(uint32_t idx) : ReferencedObject()
        {
            this->mem = Memory::Malloc("tobj2", 16);
            this->idx = idx;
        };

        uint32_t get_idx(void) { return this->idx; };
        void destroy()
        {
            Memory::Free(this->mem);
            delete this;
        }
    private:
        uint32_t idx;
        void *mem;
};

void test3(void)
{
    PStack<TestObject1> stk(1024);

    assert(stk.empty());

    for (int i = 0; i < 1024; i++)
    {
        TestObject1 *ts = new TestObject1(i);
        assert(stk.push(ts) == true);
    }

    assert(stk.size() == 1024);
    assert(!stk.empty());

    TestObject1 *ts = new TestObject1(1024);
    assert(stk.push(ts) == false);
    delete ts;
    for (int i = 1023; i >= 0; i--)
    {
        TestObject1 *ts = stk.pop();
        assert(ts);
        assert(ts->get_idx() == (uint32_t)i);
        delete ts;
    }
    assert(stk.size() == 0);
    assert(stk.empty());
    std::cout << "test3() pstack: PASSED!\n";
}

void test4(void)
{
    PQueue<TestObject1> queue(1024);

    assert(queue.empty());

    for (int i = 0; i < 1024; i++)
    {
        TestObject1 *ts = new TestObject1(i);
        assert(queue.enqueue(ts) == true);
    }

    assert(queue.size() == 1024);
    assert(!queue.empty());

    TestObject1 *ts = new TestObject1(1024);
    assert(queue.enqueue(ts) == false);
    delete ts;
    for (int i = 0; i < 1024; i++)
    {
        TestObject1 *ts = queue.dequeue();
        assert(ts);
        assert(ts->get_idx() == (uint32_t)i);
        delete ts;
    }
    assert(queue.size() == 0);
    assert(queue.empty());
    std::cout << "test4() pqueue: PASSED!\n";
}

void test5(void)
{
    PTRStack<TestObject2> stk(1024);

    assert(stk.empty());

    for (int i = 0; i < 1024; i++)
    {
        TestObject2 *ts = new TestObject2(i);
        assert(stk.push(ts) == true);
    }
    assert(stk.size() == 1024);
    assert(!stk.empty());

    TestObject2 *ts = new TestObject2(1024);
    assert(stk.push(ts) == false);
    ts->ref_del();

    for (int i = 1023; i >= 0; i--)
    {
        TestObject2 *ts = stk.pop();
        assert(ts);
        assert(ts->get_idx() == (uint32_t)i);
        ts->ref_del();
    }
    assert(stk.size() == 0);
    assert(stk.empty());
    std::cout << "test5() ptrstack: PASSED!\n";
}

void test6(void)
{
    PTRQueue<TestObject2> queue(1024);

    assert(queue.empty());

    for (int i = 0; i < 1024; i++)
    {
        TestObject2 *ts = new TestObject2(i);
        assert(queue.enqueue(ts) == true);
    }

    assert(queue.size() == 1024);
    assert(!queue.empty());

    TestObject2 *ts = new TestObject2(1024);
    assert(queue.enqueue(ts) == false);
    ts->ref_del();
    for (int i = 0; i < 1024; i++)
    {
        TestObject2 *ts = queue.dequeue();
        assert(ts);
        assert(ts->get_idx() == (uint32_t)i);
        ts->ref_del();
    }
    assert(queue.size() == 0);
    assert(queue.empty());

    for (int i = 0; i < 533; i++)
    {
        TestObject2 *ts = new TestObject2(i);
        assert(queue.enqueue(ts) == true);
    }

    assert(queue.size() == 533);
    assert(!queue.empty());

    for (int i = 0; i < 533; i++)
    {
        TestObject2 *ts = queue.dequeue();
        assert(ts);
        assert(ts->get_idx() == (uint32_t)i);
        ts->ref_del();
    }
    assert(queue.size() == 0);
    assert(queue.empty());

    ts = queue.dequeue();
    assert(ts == NULL);

    std::cout << "test6() ptrqueue: PASSED!\n";
}

class TestObject4 : public ReferencedObject
{
    public:
        TestObject4() { val = 0; }
        ~TestObject4() {}

        static const int ITERATIONS = 250000000;
        uint64_t compute_val (uint64_t *rnd)
        {
            std::lock_guard<Spinlock> lck(lock);
            uint64_t rand_val = *rnd;
            *rnd = val;
            for (int i = 0; i < ITERATIONS; i++)
            {
                add_val(rand_val);
            }
            return val;
        };

        uint64_t add_val(uint64_t x)
        {
            val += x;
            return val;
        }

        uint64_t get_val(void) { return val; };
    private:
        Spinlock lock;
        volatile uint64_t val;
};

void thread_func(void* arg)
{
    TestObject4 *t = static_cast<TestObject4*>(arg);
    t->ref_add();
    uint64_t rndval = rand() % 32;
    uint64_t in_val;
    uint64_t out_val;

    for (int i=0; i < 6; i++)
    {
        in_val = rndval;
        out_val = t->compute_val(&in_val);
        assert(out_val == (in_val + (rndval * TestObject4::ITERATIONS)));
        std::cout << "Thread (" << rndval << ") iteration complete...\n";
    }
    t->ref_del();
}

void test7(void)
{
    srand(time(NULL));
    TestObject4 *t1 = new TestObject4();
    TestObject4 *t2 = new TestObject4();
    std::thread th1(thread_func, t1);
    std::thread th2(thread_func, t1);
    std::thread th3(thread_func, t2);
    std::thread th4(thread_func, t2);
    std::thread th5(thread_func, t1);
    std::thread th6(thread_func, t1);
    std::thread th7(thread_func, t2);
    std::thread th8(thread_func, t2);

    th1.join();
    th2.join();
    th3.join();
    th4.join();
    th5.join();
    th6.join();
    th7.join();
    th8.join();
    t1->ref_del();
    t2->ref_del();
    std::cout << "test7() ptrqueue: PASSED!\n";
}

int main(int argc, char **argv)
{
    //test1();
    //test2();
    //test3();
    //test4();
    test5();
    //test6();
    //test7();
    Memory::Check();
    std::cout << "UNITTEST PASSED!\n";
    return 0;
}
