#pragma once
#include <string>
#include <memory>

typedef uint32_t event_t;

class EventReceiver;

class EventSource : public enable_shared_from_this
{
    public:
        EventSource();
        ~EventSource();

        void register_event_receiver(std::shared_ptr<EventReceiver> rcv);
        void trigger_event(std::shared_ptr<EventReceiver> rcv, event_t event);

    private:
        std::list<std::shared_ptr<EventReceiver>> receivers;
};

class EventReceiver
{
    public:
        virtual void rcv_event( std::shared_ptr<EventSource> esrc,
                                event_t event)=0;
};
