#pragma once
#include <cstdint>
#include "pstack.h"
#include "object.h"

//
// Implementation of a stack for referenced objects.
template<class T>
class PTRStack : public PStack<T>
{
    public:
        PTRStack(uint32_t size) : PStack<T>(size) {};
        ~PTRStack() {};

        bool push(T *ptr)
        {
            if (PStack<T>::push(ptr))
            {
                ptr->ref_add();
                return true;
            }
            return false;
        };

        T* pop(void)
        {
            T* obj = PStack<T>::pop();
            if (obj != NULL)
                obj->ref_del();
            return obj;
        };
};
