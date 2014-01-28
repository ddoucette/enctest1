#include <iostream>
#include "boost/assert.hpp"
#include "logger.h"
#include "i_memory.h"

class testclass : public Object
{
    public:
        testclass(std::string name) : Object(name) {};
        ~testclass() {};

        void testlog()
        {
            Logger::GetInstance()->set_level(Logger::LVL_DEBUG);
            mLOG_ERROR("This is an error! %s:%d", "hello", 11);
            mLOG_INFO("This is info! %s", "info:");
            mLOG_DEBUG("This is debug! %d:%d", 22, 33);
            BOOST_ASSERT(Logger::GetInstance()->get_nr_messages() == 3);
            Logger::GetInstance()->set_level(Logger::LVL_INFO);
            mLOG_ERROR("This is an error! %s:%d", "hello", 11);
            mLOG_INFO("This is info! %s", "info:");
            mLOG_DEBUG("This is debug! %d:%d", 22, 33);
            // Verify we do not print out the DEBUG message
            BOOST_ASSERT(Logger::GetInstance()->get_nr_messages() == 5);
        };
};

int main(int argc, char **argv)
{
    class testclass* tc = new testclass("tc1");
    tc->testlog();

    void *mem = IMemory::Malloc(1);
    BOOST_ASSERT(mem != NULL);

    std::cout << "PASSED!\n";
    return 0;
}
