
#include <cstdint>
#include <mutex>
#include "memory.h"

#define CACHELINE_BYTES     (64)

class MemoryPool;

typedef struct chunk_header
{
    union
    {
        struct header
        {
            uint32_t magic;
            void *next;
            void *prev;
            MemoryPool *pool;
        };
        uint8_t cacheline[CACHELINE_BYTES];
    };
} chunk_header_t;

class MemoryPool
{
    public:
        MemoryPool(uint32_t chunk_size);
        ~MemoryPool();

        uint32_t get_chunk_size(void) { return this->chunk_size; };

        void* malloc(uint32_t nr_chunks);
        void free(void *mem);

    private:
        void *slab;
        uint32_t slab_size;
        uint32_t chunk_size;
        std::mutex lock;
};

class MemoryManager : public Memory
{
    public:
        static MemoryManager* GetInstance(void);

        void* malloc(uint32_t size);
        void free(void *mem);

    private:
        MemoryManager();
        ~MemoryManager();
};

static MemoryManager* g_instance = NULL;

MemoryManager::MemoryManager(void)
{
}

MemoryManager::~MemoryManager(void)
{
}

MemoryManager* MemoryManager::GetInstance(void)
{
    if (g_instance == NULL)
        g_instance = new MemoryManager();
    return g_instance;
}

void* MemoryManager::malloc(uint32_t size)
{
    return NULL;
}

void MemoryManager::free(void *mem)
{
}

void* Memory::Malloc(uint32_t size)
{
    return MemoryManager::GetInstance()->malloc(size);
}

void Memory::Free(void *mem)
{
    MemoryManager::GetInstance()->free(mem);
}
