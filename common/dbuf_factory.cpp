#include "dbuf_factory.h"
#include "dbuf.h"

typedef std::shared_ptr<MemoryPool<DBuf>> dbuf_pool_t;
typedef std::shared_ptr<MemoryPool<dbuf_buffer_t>> dbuf_buffer_pool_t;

static dbuf_pool_t g_dbuf_pool(new MemoryPool<DBuf>("dbuf-mem-pool", 20));
static dbuf_buffer_pool_t g_buf_pool(new MemoryPool<dbuf_buffer_t>("dbuf-buf-pool", 20));

dbuf_t DBufFactory::Create(void)
{
    return dbuf_t(new DBuf());
}

void DBufFactory::Finalize(void)
{
    g_dbuf_pool = NULL;
    g_buf_pool = NULL;
}

void* DBuf::operator new(size_t size)
{
    mASSERT(size == sizeof(DBuf));
    return (void*)g_dbuf_pool->alloc();
}

void DBuf::operator delete(void *mem)
{
    g_dbuf_pool->free((DBuf*)mem);
}

void DBuf::alloc_buffer(void)
{
    this->mem = (uint8_t*)g_buf_pool->alloc();
}

void DBuf::free_buffer(void)
{
    g_buf_pool->free((dbuf_buffer_t*)this->mem);
}
