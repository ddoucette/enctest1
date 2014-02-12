
#pragma once
#include <cstdint>
#include <string>

class Memory
{
    public:
        static void* Malloc(std::string tag, uint32_t size);
        static void Free(void *mem);
        static void Check(void);

        // DBuf memory allocation
        static void* DBufAlloc(size_t size);
        static void DBufFree(void *mem);

        static uint8_t* DBufDataAlloc(size_t size);
        static void DBufDataFree(uint8_t *mem);
};
