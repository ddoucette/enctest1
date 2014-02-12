#pragma once
#include <memory>
#include <cassert>
#include <queue>
#include <mutex>
#include <cassert>
#include "object.h"
#include "Events.h"
#include "Stats.h"
#include "memory.h"

//
// Data buffers are the base class for all data movement within the
// codec.  They move encoded/decoded jpeg data, packet data, keyboard and
// mouse strokes.

class DataBuffer : ReferencedObject
{
    public:
        static DataBuffer* Create(uint32_t buffer_size)
        {
            return new DataBuffer(buffer_size);
        };

        uint32_t get_size(void) { return this->size; };
        void set_size(uint32_t size)
        {
            assert(size <= this->max_size);
            this->size = size;
        };
        uint8_t* get_data_address(void) { return this->buffer; };

    private:
        DataBuffer(uint32_t buffer_size)
        {
            this->buffer_size = buffer_size;
            this->size = 0;
            this->buffer = Memory::Malloc("dbuf", buffer_size);
        }
        ~DataBuffer()
        {
            Memory::Free(this->buffer);
            this->buffer = NULL;
        }
        uint32_t size;
        uint32_t buffer_size;
        uint8_t *buffer;
};

class DataReceiver : public Object, EventSource, StatsProvider
{
    public:
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

        static std::shared_ptr<DataReceiver> Create(std::name name)
        {
            return std::make_shared<DataReceiver>(name);
        };

        // Set the full threshold value.
        void set_full_threshold(uint32_t threshold);
        void set_ready_threshold(uint32_t threshold);

        // Push a data buffer into this receiver.
        // If the current queue level is equal to or beyond
        // the full mark, this will return false, but the
        // packet is accepted regardless.
        bool send(std::shared_ptr<DataBuffer> *dbuf);

        // Pull a buffer out.
        std::shared_ptr<DataBuffer> recv(void);

        // Query the state of this receiver
        bool is_ready(void);

    private:
        DataReceiver(std::string name);
        ~DataReceiver();

        std::Queue<DataBuffer>  queue;
        uint32_t                full_threshold;
        uint32_t                ready_threshold;
        std::mutex              lock;
};

class DataSource : public Object, EventReceiver, StatsProvider
{
    public:
        static std::shared_ptr<DataSource> Create(std::name name)
        {
            return std::make_shared<DataSource>(name);
        };

        // (Dis)connect this source to a data receiver.
        // Connects this data source as an event receiver as well.
        bool connect(DataReceiver *dsrc);
        void disconnect(DataReceiver *dsrc);

        // Push data through to the data receiver(s)
        bool send(std::shared_ptr<DataBuffer> dbuf);

        // Status of the data source.  If any attached sink 
        bool is_ready(void);

        // EventReceiver interface.  Accept and process events
        // from the data receivers.
        void event_rcv(std::shared_ptr<EventSource> src, event_t event);

    private:
        DataSource(std::string name);
        ~DataSource();

        std::mutex lock;
        bool in_list(std::shared_ptr<DataReceiver>);
        std::list<std::shared_ptr<DataReceiver>> receivers;
};
