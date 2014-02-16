
#pragma once
#include <cstdint>
#include <string>

class Memory
{
    public:
        static void* Malloc(std::string tag, uint32_t size);
        static void Free(void *mem);
        static void Check(void);
        static void Finalize(void);
};
