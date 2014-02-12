#pragma once

#include <string>
#include <atomic>
#include "iassert.h"

class Object
{
    public:
        Object(std::string name){ this->name = name; };
        ~Object() {};
        std::string get_name() { return this->name; };
    private:
        std::string name;
};

class ReferencedObject
{
    public:
        ReferencedObject()
        {
            val = 1;
        };
        void ref_add(void)
        {
            mASSERT(val > 0);
            val.fetch_add(1);
        };
        void ref_del(void)
        {
            mASSERT(val >= 1);
            if (val.fetch_sub(1) == 1)
            {
                destroy();
            }
        };
        virtual void destroy(void) {};
    protected:
        ~ReferencedObject()
        {
            mASSERT(val == 0);
        };

    private:
        std::atomic<int> val;
};
