
#include <cstdint>
#include <mutex>
#include <memory>
#include <list>
#include <string>
#include "memory.h"
#include "logger.h"
#include "constants.h"
#include "macros.h"
#include "iassert.h"


#define mMEM_TO_ALLOCATION(mem) \
    (mem_allocation_t*)((uintptr_t)(mem) - sizeof(mem_allocation_t))

#define mALLOCATION_TO_MEM(alloc) \
    (void*)((alloc)+1)

#define MAX_TAG_LENGTH  (CACHELINE_BYTES - sizeof(uint32_t))

typedef struct allocation
{
    uint32_t size;
    char tag[MAX_TAG_LENGTH];
} mem_allocation_t;

class MemoryManager : public Memory, Object
{
    public:
        static MemoryManager* GetInstance(void);

        void* malloc(std::string tag, uint32_t size);
        void free(void *mem);
        uint32_t nr_allocations(void) { return allocations.size(); };
        void check_allocations(void);

        MemoryManager(std::string name);
        ~MemoryManager();
    private:
        std::list<mem_allocation_t*> allocations;
};

static MemoryManager* g_instance = NULL;

MemoryManager::MemoryManager(std::string name) :
    Object(name)
{
    mCOMPILE_ASSERT(sizeof(mem_allocation_t) == CACHELINE_BYTES);
}

MemoryManager::~MemoryManager(void)
{
    check_allocations();
}

MemoryManager* MemoryManager::GetInstance(void)
{
    if (g_instance == NULL)
        g_instance = new MemoryManager("memmgr");
    return g_instance;
}

void* MemoryManager::malloc(std::string tag, uint32_t size)
{
    void *mem;
    mASSERT(tag.length() < MAX_TAG_LENGTH);

    size += sizeof(mem_allocation_t);
    if (posix_memalign(&mem, CACHELINE_BYTES, size) > 0)
    {
        mLOG_ERROR("Could not allocate (%d) bytes aligned at (%d)",
                    size, CACHELINE_BYTES);
        return NULL;
    }
    mASSERT(mem != NULL);
    mASSERT(((uintptr_t)mem & (CACHELINE_BYTES-1)) == 0);
    mem_allocation_t *alloc = (mem_allocation_t*)mem;
    tag.copy(alloc->tag, CACHELINE_BYTES);
    alloc->size = size - sizeof(mem_allocation_t);
    alloc->tag[tag.length()] = '\0';
    allocations.push_front(alloc);
    mem = mALLOCATION_TO_MEM(alloc);
    return mem;
}

void MemoryManager::free(void *mem)
{
    mem_allocation_t *alloc = mMEM_TO_ALLOCATION(mem);
    allocations.remove(alloc);
    mem = (void*)alloc;
    ::free(mem);
}

void MemoryManager::check_allocations(void)
{
    for ( std::list<mem_allocation_t*>::iterator it = allocations.begin();
          it != allocations.end();
          it++ )
    {
        mem_allocation_t *alloc = *it;
        mLOG_ERROR(" -- allocation (%s) (%d) bytes", alloc->tag, alloc->size);
    }
    mASSERT(allocations.size() == 0);
}

void* Memory::Malloc(std::string tag, uint32_t size)
{
    return MemoryManager::GetInstance()->malloc(tag, size);
}

void Memory::Free(void *mem)
{
    MemoryManager::GetInstance()->free(mem);
}

void Memory::Check(void)
{
    return MemoryManager::GetInstance()->check_allocations();
}

void Memory::Finalize(void)
{
    if (g_instance != NULL)
        delete g_instance;
    g_instance = NULL;
}
