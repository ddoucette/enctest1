#include "dbuf.h"
#include "memory_pool.h"

//MemoryPool<DBuf>* DBufPool::dbuf_pool = new MemoryPool<DBuf>(DBUF_POOL_SIZE);
static MemoryPool<DBuf>* dbuf_pool = NULL;
//MemoryPool<DBuf> *DBufPool::dbuf_pool;
//MemoryPool<dbuf_buffer_t>* DBufPool::buffer_pool = new MemoryPool<dbuf_buffer_t>(DBUF_POOL_SIZE);
static MemoryPool<dbuf_buffer_t>* buffer_pool = NULL;
//MemoryPool<dbuf_buffer_t> *DBufPool::buffer_pool;

void DBuf::Initialize(void)
{
    if (dbuf_pool == NULL)
        dbuf_pool = new MemoryPool<DBuf>(DBUF_POOL_SIZE);
    if (buffer_pool == NULL)
        buffer_pool = new MemoryPool<dbuf_buffer_t>(DBUF_POOL_SIZE);
}

DBuf::DBuf(void)
{
    this->buffer_size = DBUF_SIZE;
    this->headroom = DBUF_HEADROOM;
    this->size = 0;
    this->mem = (uint8_t*)buffer_pool->alloc();
    this->data_head = this->mem + headroom;
}

DBuf::~DBuf()
{
    buffer_pool->free((dbuf_buffer_t*)this->mem);
    this->mem = NULL;
    this->data_head = NULL;
}

void* DBuf::operator new (size_t size)
{
    return (void*)dbuf_pool->alloc();
}

void DBuf::operator delete (void *mem)
{
    dbuf_pool->free((DBuf*)mem);
}
