#include "Events.h"
#include <string>
#include <memory>
#include <mutex>
#include <list>

void EventSource::register_event_receiver(std::shared_ptr<EventReceiver> rcv)
{
    std::lock_guard<std::mutex> lck(this->lock);
    receivers.push_back(rcv);
}

void EventSource::trigger_event(event_t event)
{
    std::list<std::shared_ptr<EventReceiver>> rcv_list;
    {
        std::lock_guard<std::mutex> lck(this->lock);

        // for (std::list<std::shared_ptr<T>>::iterator it = this->receivers.begin();
        for (std::list<std::shared_ptr<EventReceiver>>::iterator it = receivers.begin();
             it != this->receivers.end();
             it++)
        {
            rcv_list.push_back(*it);
        }
    }

    // Deliver events outside of the event lock.  To do otherwise invites
    // deadlock issues.
    while (!rcv_list.empty())
    {
        std::shared_ptr<EventReceiver> rcv = rcv_list.front();
        rcv->event_rcv(this->shared_from_this(), event);
        rcv_list.pop_front();
    }
}
