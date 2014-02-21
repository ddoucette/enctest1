#pragma once

#include <memory>
#include <string>
#include <atomic>
#include "iassert.h"

class Object;
typedef std::shared_ptr<Object> object_t;

class Object
{
    public:
        Object(std::string name, object_t parent)
        {
            this->name = name;
            this->parent = parent;
        };
        ~Object() {};

        std::string get_name() { return this->name; };
        object_t get_parent() { return this->parent; };
    private:
        object_t parent;
        std::string name;
};
