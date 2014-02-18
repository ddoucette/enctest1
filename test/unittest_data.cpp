#include <iostream>
#include <memory>
#include <string>
#include "logger.h"
#include "Benchmarking.h"
#include "SurfaceEncoder.h"
#include "ISurface.h"

void test1(void)
{
    isurface_t surface = ISurface::Create("test-surface-1");
    uint32_t width = 1000;
    uint32_t height = 1000;
    uint32_t bpp = 32;
    uint32_t x = 0;
    uint32_t y = 0;
    surface->set_dimension(width, height, bpp);
    surface->set_topology(x, y);
    surface_encoder_t se1 = SurfaceEncoder::Create("Desktop1", surface);
    se1->encode();
    se1 = NULL;
}

int main(int argc, char **argv)
{
    test1();
    Memory::Check();
    std::cout << "UNITTEST PASSED!\n";
    return 0;
}
