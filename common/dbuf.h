#pragma once

#include <memory>
#include "object.h"
#include "memory_pool.h"
#include "iassert.h"
#include "ptrstack.h"

#define DBUF_SIZE       (2048)
#define DBUF_HEADROOM   (64)
#define DBUF_POOL_SIZE  (4096)

typedef struct dbuf_buffer
{
    uint8_t     buffer[DBUF_SIZE];
} dbuf_buffer_t;

class DBuf
{
    public:
        static std::shared_ptr<DBuf> Create(void)
        {
            std::shared_ptr<DBuf> dbuf(new DBuf());
            return dbuf;
        }

        static void Initialize(void);

        size_t get_size(void) { return this->size; };
        size_t get_headroom(void) { return this->headroom; };

        void set_size(size_t size)
        {
            mASSERT(size <= this->buffer_size);
            this->size = size;
        };

        uint8_t* get_data_address(void) { return this->data_head; };

        // Add/remove bytes from the front of the buffer
        void add_to_front(uint32_t nr_bytes)
        {
            mASSERT(nr_bytes <= this->headroom);
            this->headroom -= nr_bytes;
            this->data_head -= nr_bytes;
            this->size += nr_bytes;
        };

        void remove_from_front(uint32_t nr_bytes)
        {
            mASSERT(nr_bytes <= this->size);
            this->headroom += nr_bytes;
            this->data_head += nr_bytes;
            this->size -= nr_bytes;
        }

        void* operator new (size_t size);
        void operator delete (void *mem);

        DBuf();
        ~DBuf();
    private:
        size_t size;
        size_t headroom;
        size_t buffer_size;
        uint8_t *data_head;
        uint8_t *mem;
};

class DBufPool
{
    public:
        static DBuf* Alloc(void);
        static dbuf_buffer_t* BufferAlloc(void);
        static void Free(DBuf* dbuf);
        static void BufferFree(dbuf_buffer_t* buffer);
        static void Initialize(void);
    private:
        static MemoryPool<DBuf> *dbuf_pool;
        static MemoryPool<dbuf_buffer_t> *buffer_pool;
};
