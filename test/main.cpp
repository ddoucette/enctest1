#include <iostream>
#include "logger.h"

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
        };
};

int main(int argc, char **argv)
{
    class testclass* tc = new testclass("tc1");
    tc->testlog();
    std::cout << "PASSED!\n";
    return 0;
}
