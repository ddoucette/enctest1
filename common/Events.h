#pragma once
#include <string>
#include <memory>
#include <mutex>
#include <list>

typedef uint32_t event_t;

class EventReceiver;
typedef std::shared_ptr<EventReceiver> event_receiver_t;
class EventSource;
typedef std::shared_ptr<EventSource> event_source_t;

class EventSource : public std::enable_shared_from_this<EventSource>
{
    public:
        EventSource() {};
        ~EventSource() {};

        void register_event_receiver(event_receiver_t rcv);
        void trigger_event(event_t event);

    private:
        std::list<event_receiver_t> receivers;
        std::mutex lock;
};

class EventReceiver
{
    public:
        virtual void event_rcv(event_source_t esrc, event_t event)=0;
};
