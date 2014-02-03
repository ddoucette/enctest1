#pragma once
#include <list>

class Notifier;

class NotifierListener
{
    public:
        virtual void notify(Notifier *n)=0;
};

class Notifier
{
    public:
        Notifier() {};
        ~Notifier()
        {
            while (!this->listeners.empty())
                this->listeners.pop_front();
        };
        void register_notifier(NotifierListener *n) { this->listeners.push_back(n); };
        void unregister_notifier(NotifierListener *n) { this->listeners.remove(n); };

        void trigger(void)
        {
            for (std::list<NotifierListener*>::iterator it = this->listeners.begin();
                 it != this->listeners.end();
                 it++)
            {
                NotifierListener *nl = static_cast<NotifierListener*>(*it);
                nl->notify(this);
            }
        };
    private:
        std::list<NotifierListener*> listeners;
};
