#include <iostream>
#include <memory>
#include <string>
#include "logger.h"
#include "Benchmarking.h"
#include "EncoderManager.h"
#include "ISurfaceTest.h"
#include "ISurfaceManagerTest.h"
#include "ProtocolConnection.h"

void test1(void)
{
    std::cout << "Test1 - Basic Application Functionality\n";

    uint32_t width = 1000;
    uint32_t height = 1000;
    uint32_t bpp = 32;
    int32_t x = 0;
    int32_t y = 0;
    isurface_manager_test_t smgr = ISurfaceManagerTest::GetInstance();
    smgr->create_surface(width, height, x, y, bpp);

    encoder_manager_t emgr = EncoderManager::GetInstance();
    EncoderManager::Finalize();
}

int main(int argc, char **argv)
{
    test1();
    Memory::Check();
    std::cout << "UNITTEST PASSED!\n";
    return 0;
}
