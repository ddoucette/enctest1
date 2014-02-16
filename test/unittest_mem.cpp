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
    Memory::Finalize();

    std::cout << "PASSED!\n";
    return 0;
}
