#pragma once
#include <cstdint>
#include "memory.h"
#include "iassert.h"

template<class T>
class PQueue
{
    public:
        PQueue(uint32_t size)
        {
            mASSERT(size > 0);
            this->queue_size = 0;
            this->max_size = size;
            // The circular queue burns a single entry when full.  Allocate 1 extra...
            ptr_list = static_cast<T**>(Memory::Malloc("queue-list", size * sizeof(T*) + 1));
            head = ptr_list;
            tail = ptr_list;
            end_ptr = ptr_list + size + 1;
        }
        ~PQueue()
        {
            if (!empty())
            {
                mLOG_ERROR("Destroying non-empty queue!");
            }
            Memory::Free(ptr_list);
        }

        inline T** ptr_increment(T** ptr)
        {
            ptr++;
            if (ptr == end_ptr)
                ptr = ptr_list;
            return ptr;
        }

        bool enqueue(T *ptr)
        {
            mASSERT(ptr != NULL);
            T** next_entry = ptr_increment(head);
            if (next_entry == tail)
            {
                /// XXX expand the queue...
                return false;
            }
            *head = ptr;
            head = next_entry;
            queue_size++;
            return true;
        };

        T* dequeue(void)
        {
            if (head == tail)
            {
                mASSERT_DEBUG(size() == 0);
                return NULL;
            }
            T** next_entry = ptr_increment(tail);
            T* entry = *tail;
            mASSERT(entry != NULL);
            tail = next_entry;
            queue_size--;
            return entry;
        };

        bool empty(void) { return head == tail; };
        uint32_t size(void) { return queue_size; };

    private:
        T** ptr_list;
        T** head;
        T** tail;
        T** end_ptr;
        uint32_t queue_size;
        uint32_t max_size;
};
