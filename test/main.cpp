#include <iostream>
#include <cassert>
#include <memory>
#include "logger.h"
#include "memory.h"
#include "notifier.h"
#include "FrameBuffer.h"

class testclass : public Object
{
    public:
        testclass(std::string name) : Object(name) {};
        ~testclass() {};

        void testlog()
        {
            mLOG_ERROR("This is an error! %s:%d", "hello", 11);
            mLOG_INFO("This is info! %s", "info:");
            mLOG_DEBUG("This is debug! %d:%d", 22, 33);
            assert(Logger::GetInstance()->get_nr_messages() == 3);
            Logger::GetInstance()->set_level(Logger::LVL_INFO);
            mLOG_ERROR("This is an error! %s:%d", "hello", 11);
            mLOG_INFO("This is info! %s", "info:");
            mLOG_DEBUG("This is debug! %d:%d", 22, 33);
            // Verify we do not print out the DEBUG message
            assert(Logger::GetInstance()->get_nr_messages() == 5);
        };
};

class testnotifier : public Notifier
{
    public:
        testnotifier() {};
        ~testnotifier() {};
};

class testnotifierlistener : public NotifierListener, Object
{
    public:
        testnotifierlistener(std::string name) : Object(name)
        {
            notified = false;
        };
        ~testnotifierlistener() {};

        bool is_notified(void) { return notified; };

        void notify(Notifier *n)
        {
            mLOG_INFO("notified!");
            notified = true;
        };
    private:
        bool notified;
};

int main(int argc, char **argv)
{
    class testclass* tc = new testclass("tc1");
    tc->testlog();

    void *mem = Memory::Malloc("main1", 1);
    assert(mem != NULL);
    assert(((uintptr_t)mem & 63) == 0);
    Memory::Free(mem);

    mem = Memory::Malloc("main2", 1<<28);
    assert(mem != NULL);
    assert(((uintptr_t)mem & 63) == 0);
    Memory::Free(mem);

    testnotifier *tgen = new testnotifier();
    testnotifierlistener *trcv1 = new testnotifierlistener("trcv1");
    testnotifierlistener *trcv2 = new testnotifierlistener("trcv2");
    testnotifierlistener *trcv3 = new testnotifierlistener("trcv3");
    testnotifierlistener *trcv4 = new testnotifierlistener("trcv4");

    tgen->register_notifier(trcv1);
    tgen->register_notifier(trcv2);
    tgen->register_notifier(trcv3);
    tgen->register_notifier(trcv4);
    tgen->trigger();
    assert(trcv1->is_notified());
    assert(trcv2->is_notified());
    assert(trcv3->is_notified());
    assert(trcv4->is_notified());

    std::shared_ptr<FrameBuffer> fb = FrameBuffer::Create("fb1", 1000,1000,32);
    assert(fb != NULL);
    assert(fb->get_width() == 1000);
    assert(fb->get_height() == 1000);
    assert(fb->get_bpp() == 32);
    fb = NULL;

    std::shared_ptr<FrameBuffer> fb2 = FrameBuffer::Create("fb2", 1000,1000,32);
    std::shared_ptr<FrameBuffer> fb3 = FrameBuffer::Create("fb3", 1000,1000,32);
    std::shared_ptr<FrameBuffer> fb4 = FrameBuffer::Create("fb4", 1000,1000,32);
    std::shared_ptr<FrameBuffer> fb5 = FrameBuffer::Create("fb5", 1000,1000,32);
    std::shared_ptr<FrameBuffer> fb6 = FrameBuffer::Create("fb6", 1000,1000,32);
    fb2 = NULL;
    fb3 = NULL;
    fb4 = NULL;
    fb5 = NULL;
    fb6 = NULL;
    Memory::Check();

    std::cout << "PASSED!\n";
    return 0;
}
