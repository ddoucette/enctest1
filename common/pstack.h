#pragma once
#include <cstdint>
#include "memory.h"
#include "logger.h"

template<class T>
class PStack
{
    public:
        PStack(uint32_t size)
        {
            this->stack_size = size;
            ptr_list = static_cast<T**>(Memory::Malloc("stack-list", size * sizeof(T*)));
            ptr_end = ptr_list + size;
            head = ptr_list;
        }
        ~PStack()
        {
            if (!empty())
            {
                mLOG_ERROR("Destroying non-empty stack!");
            }
            Memory::Free(ptr_list);
        }

        bool push(T *ptr)
        {
            if (head != ptr_end)
            {
                *head = ptr;
                head++;
                return true;
            }
            /// else expand the stack...
            return false;
        };

        T* pop(void)
        {
            if (head != ptr_list)
            {
                head--;
                return *head;
            }
            return NULL;
        };

        bool empty(void) { return head == ptr_list; };

        uint32_t size(void)
        {
            return (uint32_t)(head - ptr_list);
        }

    private:
        T** ptr_list;
        T** ptr_end;
        T** head;
        uint32_t stack_size;
};
