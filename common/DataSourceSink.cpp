#include "DataSourceSink.h"
#include <memory>
#include <list>
#include <cassert>
#include "memory.h"

std::shared_ptr<DataBuffer> DataBuffer::Create(uint32_t buffer_size)
{
    return std::shared_ptr<DataBuffer> dbuf(new DataBuffer(buffer_size));
}

DataBuffer::DataBuffer(uint32_t buffer_size)
{
    this->size = 0;
    this->buffer_size = buffer_size;
    this->buffer = (uint8_t*)Memory::Malloc("dbuf", buffer_size);
}

DataBuffer::~DataBuffer()
{
    if (this->buffer != NULL)
    {
        Memory::Free(this->buffer);
        this->buffer = NULL;
    }
}

// DataReceiver

std::shared_ptr<DataReceiver> DataReceiver::Create(std::name name)
{
    return std::shared_ptr<DataReceiver> drcv(new DataReceiver(name));
}

DataReceiver::DataReceiver(std::string name) :
                           Object(name),
                           StatsProvider(name)
{
    this->full_threshold = 0xffffffff;
    this->ready_threshold = 0;
}

DataReceiver::~DataReceiver()
{
    /*
     * XXX
     * It may be possible to signal the 'DATA_READY' event
     * here if there are buffers still in the queue in hopes
     * of draining it properly.
     */
    while (!queue.empty())
        queue.pop();
}

void DataReceiver::set_full_threshold(uint32_t threshold)
{
    std::lock_guard<std::mutex> lck(this->lock);
    uint32_t old_threshold = this->full_threshold;

    // If our newly set full threshold is less than our
    // queue size, we need to send out a FULL event.
    // If changing the threshold here creates more room,
    // we should send out the READY event.
    this->full_threshold = threshold;
    if (this->full_threshold <= queue.size())
    {
        trigger_event(EVENT_RCV_FULL);
    }
    else if (old_threshold <= queue.size())
    {
        trigger_event(EVENT_RCV_READY);
    }
}

void DataReceiver::send(std::shared_ptr<DataBuffer> *dbuf)
{
    std::lock_guard<std::mutex> lck(this->lock);

    queue.push(dbuf);

    // Stats
    stat_timestamp();
    stat_increment(STAT_RCV_PKTS, 1);
    stat_increment(STAT_RCV_BYTES, dbuf->get_size());

    if (queue.size() == full_threshold)
        trigger_event(EVENT_RCV_FULL);
}

bool DataReceiver::is_ready(void)
{
    std::lock_guard<std::mutex> lck(this->lock);
    return (queue.size() >= full_threshold);
}

class DataSource : public Object, EventReceiver, StatsProvider
{
    public:
        static std::shared_ptr<DataSource> Create(std::name name);

        // (Dis)connect this source to a data receiver.
        // Connects this data source as an event receiver as well.
        bool connect(DataReceiver *dsrc);
        void disconnect(DataReceiver *dsrc);

        // Push data through to the data receiver(s)
        bool send(DataBuffer *dbuf);

        // Status of the data source.  If any attached sink 
        bool is_ready(void);

    private:
        DataSource(std::string name);
        ~DataSource();
        std::list<std::shared_ptr<DataReceiver>> receivers;
};
