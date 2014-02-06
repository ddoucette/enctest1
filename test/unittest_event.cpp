#include <iostream>
#include <cassert>
#include <memory>
#include <string>
#include "object.h"
#include "logger.h"
#include "Events.h"
#include "Benchmarking.h"

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
}

int main(int argc, char **argv)
{
    test1();
    test2();
    std::cout << "UNITTEST PASSED!\n";
    return 0;
}
