#include <iostream>
#include <memory>
#include <string>
#include "logger.h"
#include "Benchmarking.h"
#include "ApplicationManager.h"
#include "ISurfaceTest.h"
#include "ISurfaceManagerTest.h"

void test1(void)
{
    std::cout << "Test1 - Basic Application Functionality\n";

    application_manager_t app = ApplicationManager::GetInstance();
    isurface_manager_test_t smgr = ISurfaceManagerTest::GetInstance();

    uint32_t width = 1000;
    uint32_t height = 1000;
    uint32_t bpp = 32;
    int32_t x = 0;
    int32_t y = 0;
    smgr->create_surface(width, height, x, y, bpp);

    ApplicationManager::Finalize();
}

int main(int argc, char **argv)
{
    test1();
    Memory::Check();
    std::cout << "UNITTEST PASSED!\n";
    return 0;
}
