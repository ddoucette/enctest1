#include <iostream>
#include <cassert>
#include <memory>
#include <string>
#include "object.h"
#include "logger.h"
#include "Events.h"
#include "Benchmarking.h"

void test1(void)
{
}

int main(int argc, char **argv)
{
    test1();
    std::cout << "UNITTEST PASSED!\n";
    return 0;
}
