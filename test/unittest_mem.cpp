#include <iostream>
#include <cassert>
#include <memory>
#include "memory.h"
#include "FrameBuffer.h"

int main(int argc, char **argv)
{
    void *mem = Memory::Malloc("main1", 1);
    assert(mem != NULL);
    assert(((uintptr_t)mem & 63) == 0);
    Memory::Free(mem);

    mem = Memory::Malloc("main2", 1<<28);
    assert(mem != NULL);
    assert(((uintptr_t)mem & 63) == 0);
    Memory::Free(mem);

    uint32_t width = 1000;
    uint32_t height = 1000;
    uint32_t bpp = 32;
    uint32_t stride = width * (bpp/8);
    frame_buffer_t fb2 = FrameBuffer::Create(width,height,bpp,stride);
    frame_buffer_t fb3 = FrameBuffer::Create(width,height,bpp,stride);
    frame_buffer_t fb4 = FrameBuffer::Create(width,height,bpp,stride);
    frame_buffer_t fb5 = FrameBuffer::Create(width,height,bpp,stride);
    frame_buffer_t fb6 = FrameBuffer::Create(width,height,bpp,stride);
    fb2 = NULL;
    fb3 = NULL;
    fb4 = NULL;
    fb5 = NULL;
    fb6 = NULL;
    Memory::Check();
    Memory::Finalize();

    std::cout << "PASSED!\n";
    return 0;
}
