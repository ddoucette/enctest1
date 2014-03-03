#pragma once

#include <memory>
#include "object.h"
#include "FrameBuffer.h"
#include "ImageTile.h"
#include "DataSource.h"
#include "Threading.h"
#include "Events.h"
#include "ProtocolConnection.h"

// SurfaceEncoder and SliceEncoder are circularly dependent.
// We need to declare the surface encoder explicitly here.
class SurfaceEncoder;
typedef std::shared_ptr<SurfaceEncoder> surface_encoder_t;

class SliceEncoder;
typedef std::shared_ptr<SliceEncoder> slice_encoder_t;

// The slice encoder will emit a 'done' event when the slice
// has completed encoding.
class SliceEncoder : public Object,
                     public EventSource,
                     public ThreadRunnable
{
    public:
        enum Event
        {
            EVENT_ENCODE_COMPLETE
        };

        static slice_encoder_t Create(
                                    surface_encoder_t encoder,
                                    frame_buffer_t fb,
                                    image_tile_source_t data_queue,
                                    uint32_t slice_nr);

        ~SliceEncoder();

        // Encode the slice.  Emits 'EVENT_ENCODE_COMPLETE' when
        // the slice is encoded and pushed out.
        void encode(void);

        // Run the encoder
        virtual void run(void);

        bool is_ready(void) { return true; }
    private:
        SliceEncoder(   std::string name,
                        surface_encoder_t surface_encoder,
                        frame_buffer_t fb,
                        image_tile_source_t dsrc,
                        uint32_t slice_nr);

        image_tile_source_t     data_queue;
        frame_buffer_t          fb;
        uint32_t                slice_nr;
};
