#pragma once
#include <memory>
#include <vector>
#include <map>

typedef enum channel_type
{
    CHANNEL_IMAGING,
    CHANNEL_CURSOR,
    CHANNEL_MOUSE,
    CHANNEL_AUDIO,
    CHANNEL_KEYBOARD,
    CHANNEL_WINDOW_COMMANDS,
    CHANNEL_TOUCH_GESTURES
} channel_type_t;
class Channel : public Object, EventSource
{
    public:

        enum Events
        {
            EVENT_TX_READY,
            EVENT_TX_FULL,
            EVENT_RX_READY
        };

        // Create a data channel
        static std::shared_ptr<Channel> Create(std::name, channel_type_t type);

        // (Un)register a data receiver (data sink)
        bool register_receiver(DataReceiver *drcv);
        void unregister_receiver(DataReceiver *drcv);

        bool tx(std::shared_ptr<ChannelData> *data);
        std::shared_ptr<ChannelData> rx(DataReceiver *drcv);

        channel_stats_t get_channel_stats(void) { return this->channel_stats; };

    private:
        Channel(std::name, channel_type_t type);
        ~Channel();
        channel_stats_t channel_stats;
};

class RemoteConnection : public Object
{
    public:
        // Create an instance
        std::shared_ptr<RemoteConnection> Create(std::name);

        // Create/add a channel to this remote connection
        bool create_channel(channel_type_t type);
        void destroy_channel(channel_type_t type);

        std::vector<channel_type_t> get_available_channel_types(void);
        std::shared_ptr<Channel> get_channel(channel_type_t type);

    private:
        RemoteConnection(std::string name);
        ~RemoteConnection(std::string name);
        std::map<channel_type_t, Channel> channels;
};
