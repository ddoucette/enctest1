#include "dbuf.h"
#include "memory_pool.h"

//MemoryPool<DBuf>* DBufPool::dbuf_pool = new MemoryPool<DBuf>(DBUF_POOL_SIZE);
static DBuf::MemoryPool<DBuf>* dbuf_pool = NULL;
//MemoryPool<DBuf> *DBufPool::dbuf_pool;
//MemoryPool<dbuf_buffer_t>* DBufPool::buffer_pool = new MemoryPool<dbuf_buffer_t>(DBUF_POOL_SIZE);
static DBuf::MemoryPool<dbuf_buffer_t>* buffer_pool = NULL;
//MemoryPool<dbuf_buffer_t> *DBufPool::buffer_pool;

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
