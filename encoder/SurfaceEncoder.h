#pragma once
#include <memory>
#include <list>
#include "ISurface.h"
#include "SliceEncoder.h"
#include "Threading.h"

class SurfaceEncoder;
typedef std::shared_ptr<SurfaceEncoder> surface_encoder_t;

class SurfaceEncoder : public Object,
                       public EventReceiver,
                       public ThreadRunnable
{
    public:
        static surface_encoder_t Create(std::string name, isurface_t surface); 

        // Start/stop encoding
        bool start(thread_pool_t thread_pool);
        void stop(void);

        // ThreadRunnable interface
        void run(void);

        // Receive events from the slice encoders
        void event_rcv(event_source_t src, event_t event);

        ~SurfaceEncoder();

    private:
        SurfaceEncoder(std::string name, isurface_t surface);

        isurface_t                  surface;
        std::list<slice_encoder_t>  slice_encoders;
        frame_buffer_t              fb;
        image_data_source_t         data_queue;
        thread_pool_t               thread_pool;
};
