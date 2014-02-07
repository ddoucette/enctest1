#include "DataSourceSink.h"
#include <memory>
#include <list>
#include <cassert>
#include "memory.h"

DataReceiver::DataReceiver(std::string name) :
                           Object(name),
                           StatsProvider(name)
{
    this->full_threshold = 0xffffffff;
    this->ready_threshold = 0;
    StatsProvider::add(STAT_RCV_PKTS, "Packets Received", "Pkts/Sec");
    StatsProvider::add(STAT_RCV_BITS, "Bits Received", "Bits/Sec");
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
    DataReceiver::Event evt = EVENT_NONE;
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
            evt = EVENT_RCV_FULL;
        }
        else if (old_threshold <= queue.size())
        {
            evt = EVENT_RCV_READY;
        }
    }
    if (evt != EVENT_NONE)
        trigger_event(evt);
}

void DataReceiver::send(std::shared_ptr<DataBuffer> *dbuf)
{
    DataReceiver::Event evt = EVENT_NONE;
    {
        std::lock_guard<std::mutex> lck(this->lock);

        queue.push(dbuf);

        // Stats
        StatsProvider::increment(STAT_RCV_PKTS, 1);
        StatsProvider::increment(STAT_RCV_BITS, dbuf->get_size() * 8);

        if (queue.size() == full_threshold)
            evt = EVENT_RCV_FULL;
    }
    if (evt != EVENT_NONE)
        trigger_event(EVENT_RCV_FULL);
}

bool DataReceiver::is_ready(void)
{
    std::lock_guard<std::mutex> lck(this->lock);
    return (queue.size() < full_threshold);
}

// DataSource
DataSource::DataSource(std::string name) :
                        Object(name),
                        StatsProvider(name)
{
}

DataSource::~DataSource()
{
    while (!receivers.empty())
        receivers.pop();
}

bool DataSource::connect(DataReceiver *dsrc)
{
    std::lock_guard<std::mutex> lck(this->lock);
    assert(!in_list(dsrc));

    receivers.push_front(dsrc);
    
    // Register to receive events from the data receiver.
    dsrc.register_event_receiver(this);
    return true;
}

void DataSource::disconnect(DataReceiver *dsrc)
{
    std::lock_guard<std::mutex> lck(this->lock);
    assert(in_list(dsrc));

    dsrc.unregister_event_listener(this);
    receivers.remove(dsrc);
}

bool DataSource::send(std::shared_ptr<DataBuffer> dbuf)
{
    std::lock_guard<std::mutex> lck(this->lock);
    for (std::list<std::shared_ptr<DataReceiver>>::iterator it = receivers.begin();
         it != receivers.end();
         it++)
    {
        std::shared_ptr<DataReceiver> dsrc = *it;
        dsrc->send(dbuf);
    }
}

void DataSource::event_rcv(std::shared_ptr<EventSource> src, event_t event)
{
}

bool DataSource::is_ready(void)
{
    std::lock_guard<std::mutex> lck(this->lock);
    for (std::list<std::shared_ptr<DataReceiver>>::iterator it = receivers.begin();
         it != receivers.end();
         it++)
    {
        std::shared_ptr<DataReceiver> dsrc = *it;
        if (!dsrc->is_ready())
            return false;
    }
    return true;
}
