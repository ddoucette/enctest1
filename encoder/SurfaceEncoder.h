#pragma once
#include <memory>
#include <list>
#include "ISurface.h"
#include "SliceEncoder.h"
#include "Threading.h"
#include "ImageTile.h"
#include "EncoderManager.h"

class SurfaceEncoder;
typedef std::shared_ptr<SurfaceEncoder> surface_encoder_t;

class SurfaceEncoder : public Object,
                       public EventReceiver,
                       public ThreadRunnable,
                       public std::enable_shared_from_this<SurfaceEncoder>
{
    public:
        static surface_encoder_t Create(isurface_t surface);

        // Start/stop encoding
        bool start(thread_pool_t thread_pool);
        void stop(void);

        // ThreadRunnable interface
        void run(void);

        // Set encoder frequency (Hz)
        bool set_frequency(uint32_t hz);

        // Receive events from the slice encoders
        void event_rcv(event_source_t src, event_t event);

        // Retrieve the surface.
        isurface_t get_surface(void) { return surface; }

        // Access the data queue.  Need this to allow receivers
        // to plug into the tile output of this encoder.
        image_tile_source_t get_data_source(void) { return data_queue; };

        ~SurfaceEncoder();

    private:
        SurfaceEncoder(std::string name, isurface_t surface);

        isurface_t                  surface;
        std::list<slice_encoder_t>  slice_encoders;
        frame_buffer_t              fb;
        image_tile_source_t         data_queue;
        thread_pool_t               thread_pool;
        uint32_t                    usec_interval;
        uint32_t                    slice_ready_count;
};
