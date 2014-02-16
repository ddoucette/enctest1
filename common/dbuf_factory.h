#pragma once
#include <memory>
#include "dbuf.h"


typedef std::shared_ptr<DBuf> dbuf_t;

class DBufFactory
{
    public:
        static dbuf_t Create(void);
        static void Finalize(void);

    private:
        DBufFactory();
        ~DBufFactory();
};
