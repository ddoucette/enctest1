
#pragma once
#include <cstdint>

class Memory
{
    public:
        static void* Malloc(uint32_t size);
        static void Free(void *mem);
};
