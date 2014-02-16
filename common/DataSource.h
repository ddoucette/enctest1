#pragma once

#include <memory>
#include <list>
#include "object.h"
#include "Events.h"
//#include "stats.h"
#include "DataReceiver.h"

//
// The DataSource object emits data packets.
// It allows any data receiver to be connected to receive
// outbound data packets, basically facilitating a simple
// broadcast/collaboration mechanism.
// This object also handles the reception and re-issue of
// ready/full events.
//
template<class T>
class DataSource :
        public Object,
        public EventReceiver,
        public std::enable_shared_from_this<DataSource<T>>
        //public StatsProvider,
{
    public:
        static std::shared_ptr<DataSource<T>> Create(std::string name)
        {
            return std::make_shared<DataSource<T>>(name);
        };

        // (Dis)connect this source to a data receiver.
        // Connects this data source as an event receiver as well.
        bool connect(std::shared_ptr<DataReceiver<T>> drcv);
        void disconnect(std::shared_ptr<DataReceiver<T>> drcv);

        // Push data through to the data receiver(s)
        bool send(std::shared_ptr<T> buf);

        // Status of the data source.  If any attached sink 
        bool is_ready(void);

        // EventReceiver interface.  Accept and process events
        // from the data receivers.
        void event_rcv(std::shared_ptr<EventSource> src, event_t event);

        DataSource(std::string name) : Object(name) {}
        ~DataSource()
        {
            while (!receivers.empty())
                receivers.pop_front();
        }

    private:
        std::mutex lock;
        bool in_list(std::shared_ptr<DataReceiver<T>>);
        std::list<std::shared_ptr<DataReceiver<T>>> receivers;
};

template<class T>
bool DataSource<T>::connect(std::shared_ptr<DataReceiver<T>> drcv)
{
    std::lock_guard<std::mutex> lck(this->lock);
    mASSERT(!in_list(drcv));

    receivers.push_front(drcv);
    
    // Register to receive events from the data receiver.
    drcv->register_event_receiver(this->shared_from_this());
    return true;
}

template<class T>
void DataSource<T>::disconnect(std::shared_ptr<DataReceiver<T>> drcv)
{
    std::lock_guard<std::mutex> lck(this->lock);
    if (in_list(drcv))
    {
        drcv->unregister_event_listener(this);
        receivers.remove(drcv);
    }
}

template<class T>
bool DataSource<T>::send(std::shared_ptr<T> buf)
{
    std::lock_guard<std::mutex> lck(this->lock);
    for (typename std::list<std::shared_ptr<DataReceiver<T>>>::iterator it = receivers.begin();
         it != receivers.end();
         it++)
    {
        std::shared_ptr<DataReceiver<T>> drcv = *it;
        drcv->push_in(buf);
    }
    return true;
}

template<class T>
void DataSource<T>::event_rcv(std::shared_ptr<EventSource> src, event_t event)
{
    mLOG_INFO("Received event (%d)", event);
}

template<class T>
bool DataSource<T>::is_ready(void)
{
    // All receivers must be ready in order for this to be ready.
    std::lock_guard<std::mutex> lck(this->lock);
    for (typename std::list<std::shared_ptr<DataReceiver<T>>>::iterator it = receivers.begin();
         it != receivers.end();
         it++)
    {
        std::shared_ptr<DataReceiver<T>> drcv = *it;
        if (!drcv->is_ready())
            return false;
    }
    return true;
}

template<class T>
bool DataSource<T>::in_list(std::shared_ptr<DataReceiver<T>> drcv)
{
    for (typename std::list<std::shared_ptr<DataReceiver<T>>>::iterator it = receivers.begin();
         it != receivers.end();
         it++)
    {
        if (drcv == *it)
            return true;
    }
    return false;
}
