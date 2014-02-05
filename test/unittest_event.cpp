#include <iostream>
#include <cassert>
#include <memory>
#include <string>
#include "object.h"
#include "logger.h"
#include "Events.h"

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
        TestEventReceiver(std::string name) : Object(name){};
        ~TestEventReceiver() {};

        void event_rcv(std::shared_ptr<TestEventSource> src, event_t event)
        {
            mLOG_INFO("Received event (%u)", event);
            events.push_back(event);
        }
        std::list<event_t> events;
};

void test1(void)
{
    //
    // 4 Test event sources, 4 test event receivers.
    // Verify:
    // 1) 1-EventSource-to-1-EventReceiver
    // 2) N-EventSource-to-1-EventReceiver
    // 3) 1-EventSource-to-N-EventReceiver
    std::shared_ptr<TestEventSource>
                        t1(new TestEventSource("t1"));
    std::shared_ptr<TestEventReceiver> r1(new TestEventReceiver("r1"));

    t1->register_event_receiver(r1);
    t1->trigger_event(TEST_EVENT_1);
    assert(r1->events.size() == 1);
    assert(r1->events.front() == TEST_EVENT_1);

/*
    std::shared_ptr<TestEventReceiver> r1(new TestEventReceiver("r1"));
    std::shared_ptr<TestEventReceiver> r2(new TestEventReceiver("r2"));
    std::shared_ptr<TestEventReceiver> r3(new TestEventReceiver("r3"));
    std::shared_ptr<TestEventReceiver> r4(new TestEventReceiver("r4"));

    t1->register_event_receiver(r1);
    t1->trigger_event(TestEventSource::TEST_EVENT_1);
    assert(r1->events.size() == 1);
    assert(r1->events.front() == TestEventSource::TEST_EVENT_1);

    t2->register_event_receiver(r2);
    t3->register_event_receiver(r2);
    t4->register_event_receiver(r2);
    t2->trigger_event(TestEventSource::TEST_EVENT_2);
    t3->trigger_event(TestEventSource::TEST_EVENT_3);
    t4->trigger_event(TestEventSource::TEST_EVENT_4);
    assert(r2->events.size() == 3);
    assert(r2->events.front() == TestEventSource::TEST_EVENT_2);
    r2->events.pop_front();
    assert(r2->events.front() == TestEventSource::TEST_EVENT_3);
    r2->events.pop_front();
    assert(r2->events.front() == TestEventSource::TEST_EVENT_4);
*/
}

int main(int argc, char **argv)
{
    test1();
    std::cout << "PASSED!\n";
    return 0;
}
