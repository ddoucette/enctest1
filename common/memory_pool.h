#pragma once

#include <string>
#include "pstack.h"
#include "spinlock.h"
#include "logger.h"
#include "constants.h"
#include "iassert.h"
#include "memory.h"
#include "object.h"

template<typename T>
class MemoryPool : public Object
{
    public:
        MemoryPool(std::string name, size_t size);
        ~MemoryPool();

        T* alloc(void);
        void free(T*);

    private:
        PStack<T>  *mem_stack;
        Spinlock    lock;
        size_t      size;
        void       *buffer;
        uint64_t    nr_allocations;
};

template<class T>
MemoryPool<T>::MemoryPool(std::string name, size_t size) :
                    Object(name)
{
    mASSERT(size > 0);

    this->size = size;
    this->nr_allocations = 0;
    mem_stack = new PStack<T>(size);

    // For performance reasons, we would like to follow a few
    // rules for buffer allocations:
    // 1) Do not have buffer allocations overlap on the same
    //     cacheline.
    // 2) For larger buffer allocations, avoid power-of-2 overlap.
    //     For these, push out the allocation size to a prime number
    //      of cachelines.
    size_t element_size = sizeof(T);
    mASSERT(element_size > 0);

    size_t cache_aligned_element_size = mALIGN(element_size, CACHELINE_BYTES);
    mASSERT(cache_aligned_element_size >= CACHELINE_BYTES);
    this->buffer = Memory::Malloc(this->get_name(), cache_aligned_element_size * size);
    T  *element = (T*)this->buffer;
    for (size_t i = 0; i < size; i++ )
    {
        mem_stack->push(element);
        element++;
    }
}

template<class T>
MemoryPool<T>::~MemoryPool(void)
{
    mASSERT(mem_stack->size() == this->size);
    Memory::Free(this->buffer);

    mLOG_INFO("(%s) nr_allocations (%lu)", this->get_name().c_str(), this->nr_allocations);

    // We dont really need to pop each item, we do it just because...
    while (!mem_stack->empty())
        mem_stack->pop();

    delete mem_stack;
}

template<class T>
T* MemoryPool<T>::alloc(void)
{
    this->lock.lock();
    T* element = mem_stack->pop();
    this->nr_allocations++;
    this->lock.unlock();
    return element;
}

template<class T>
void MemoryPool<T>::free(T* mem)
{
    this->lock.lock();
    mem_stack->push(mem);
    this->lock.unlock();
}
