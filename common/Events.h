#pragma once
#include <string>
#include <memory>
#include <mutex>
#include <list>

typedef uint32_t event_t;

class EventReceiver;

class EventSource : public std::enable_shared_from_this<EventSource>
{
    public:
        EventSource() {};
        ~EventSource() {};

        void register_event_receiver(std::shared_ptr<EventReceiver> rcv);
        void trigger_event(event_t event);

    private:
        std::list<std::shared_ptr<EventReceiver>> receivers;
        std::mutex lock;
};

class EventReceiver
{
    public:
        virtual void event_rcv( std::shared_ptr<EventSource> esrc,
                                event_t event)=0;
};
