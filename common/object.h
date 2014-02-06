
#pragma once

#include <string>

class Object
{
    public:
        Object(std::string name){ this->name = name; };
        ~Object() {};
        std::string get_name() { return this->name; };
    private:
        std::string name;
};
