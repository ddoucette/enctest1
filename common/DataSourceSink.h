#pragma once
#include <memory>
#include <cassert>
#include <queue>
#include <mutex>
#include "object.h"
#include "event.h"
#include "stats.h"

class DataBuffer
{
    public:
        static std::shared_ptr<DataBuffer> Create(uint32_t buffer_size);

        uint32_t get_size(void) { return this->size; };
        void set_size(uint32_t size)
        {
            assert(size <= this->max_size);
            this->size = size;
        };
        uint8_t* get_data_address(void) { return this->buffer; };

    private:
        DataBuffer(uint32_t buffer_size);
        ~DataBuffer();
        uint32_t size;
        uint32_t buffer_size;
        uint8_t *buffer;
};

class DataReceiver : public Object, EventSource, StatsProvider
{
    public:
        // Event values propagated via the EventSource interface.
        enum
        {
            EVENT_RCV_READY,
            EVENT_RCV_FULL,
            EVENT_DATA_READY
        };

        static std::shared_ptr<DataReceiver> Create(std::name name);

        // Set the full threshold value.
        void set_full_threshold(uint32_t threshold);
        void set_ready_threshold(uint32_t threshold);

        // Push a data buffer into this receiver.
        // There is no return value, this will always work.
        void send(std::shared_ptr<DataBuffer> *dbuf);

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
        static std::shared_ptr<DataSource> Create(std::name name);

        // (Dis)connect this source to a data receiver.
        // Connects this data source as an event receiver as well.
        bool connect(DataReceiver *dsrc);
        void disconnect(DataReceiver *dsrc);

        // Push data through to the data receiver(s)
        bool send(std::shared_ptr<DataBuffer> dbuf);

        // Status of the data source.  If any attached sink 
        bool is_ready(void);

    private:
        DataSource(std::string name);
        ~DataSource();

        std::mutex lock;
        bool in_list(std::shared_ptr<DataReceiver>);
        std::list<std::shared_ptr<DataReceiver>> receivers;
};
