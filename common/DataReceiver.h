#pragma once
#include <memory>
#include <queue>
#include "object.h"
#include "Events.h"
#include "Stats.h"
#include "memory.h"
#include "dbuf.h"

//
// DataReceiver is basically a packet queue that handles
// ready/full event propagation in the same thread context.
//
class DataReceiver;
typedef std::shared_ptr<DataReceiver<T>> data_receiver_t<T>;

template<class T>
class DataReceiver :
            public Object,
            public EventSource
            //public StatsProvider
{
    public:
        static std::shared_ptr<DataReceiver<T>> Create(
                                            std::string name,
                                            size_t full_threshold,
                                            size_t ready_threshold)
        {
            std::shared_ptr<DataReceiver<T>> ptr(
                            new DataReceiver<T>(name, full_threshold, ready_threshold));
            return ptr;
        }

        // Event values propagated via the EventSource interface.
        enum Event
        {
            EVENT_RCV_READY,
            EVENT_RCV_FULL,
            EVENT_DATA_READY,
            EVENT_NONE = -1
        };

        enum Stats
        {
            STAT_RCV_PKTS,
            STAT_RCV_BYTES
        };

        // Push a data buffer into this receiver.
        // If the current queue level is equal to or beyond
        // the full mark, this will return false, but the
        // packet is accepted regardless.
        bool push_in(std::shared_ptr<T> dbuf);

        // Pull a buffer out.
        std::shared_ptr<T> pull_out(void);

        // Query the state of this receiver
        bool is_ready(void)
        {
            std::lock_guard<std::mutex> lck(this->lock);
            return (queue.size() < full_threshold);
        }

        DataReceiver(std::string name, size_t full_threshold, size_t ready_threshold) :
                                            Object(name)
        {
            this->full_threshold = full_threshold;
            this->ready_threshold = ready_threshold;
            //StatsProvider::add(STAT_RCV_PKTS, "Packets Received", "Pkts/Sec");
            //StatsProvider::add(STAT_RCV_BITS, "Bits Received", "Bits/Sec");
        }

        ~DataReceiver()
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

    private:
        std::queue<std::shared_ptr<T>>  queue;
        std::mutex                      lock;
        size_t                          full_threshold;
        size_t                          ready_threshold;
};

template<class T>
bool DataReceiver<T>::push_in(std::shared_ptr<T> dbuf)
{
    DataReceiver<T>::Event evt = EVENT_NONE;
    {
        std::lock_guard<std::mutex> lck(this->lock);

        queue.push(dbuf);

        // Stats
        //StatsProvider::increment(STAT_RCV_PKTS, 1);
        //StatsProvider::increment(STAT_RCV_BITS, dbuf->get_size() * 8);

        if (queue.size() >= full_threshold)
            evt = EVENT_RCV_FULL;
    }
    // Trigger event outside of lock
    if (evt != EVENT_NONE)
        trigger_event(evt);
    return true;
}

template<class T>
std::shared_ptr<T> DataReceiver<T>::pull_out(void)
{
    DataReceiver<T>::Event evt = EVENT_NONE;
    std::shared_ptr<T> ptr(NULL);
    {
        std::lock_guard<std::mutex> lck(this->lock);
        if (queue.empty())
            return NULL;

        std::shared_ptr<T> ptr = queue.pop();
        if (queue.size() == ready_threshold)
            evt = EVENT_RCV_READY;
    }
    if (evt != EVENT_NONE)
        trigger_event(evt);
    return ptr;
}
