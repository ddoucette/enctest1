#pragma once
#include <cstdint>
#include "pqueue.h"
#include "object.h"

//
// Implementation of a queue for referenced objects.
template<class T>
class PTRQueue : public PQueue<T>
{
    public:
        PTRQueue(uint32_t size) : PQueue<T>(size) {};
        ~PTRQueue() {};

        bool enqueue(T *ptr)
        {
            if (PQueue<T>::enqueue(ptr))
            {
                ptr->ref_add();
                return true;
            }
            return false;
        };

        T* dequeue(void)
        {
            T* obj = PQueue<T>::dequeue();
            if (obj != NULL)
                obj->ref_del();
            return obj;
        };
};
