#pragma once

#include <memory>
#include "object.h"
#include "FrameBuffer.h"
#include "ImageTile.h"
#include "DataSource.h"
#include "Events.h"

typedef std::shared_ptr<DataSource<ImageTile>> image_data_source_t;
class SliceEncoder;
typedef std::shared_ptr<SliceEncoder> slice_encoder_t;

// The slice encoder will emit a 'done' event when the slice
// has completed encoding.
class SliceEncoder : public Object,
                     public EventSource
{
    public:
        enum Event
        {
            EVENT_ENCODE_COMPLETE
        };

        static slice_encoder_t Create(
                                    frame_buffer_t fb,
                                    image_data_source_t data_queue,
                                    uint32_t slice_nr);

        ~SliceEncoder();

        // Encode the slice.  Emits 'EVENT_ENCODE_COMPLETE' when
        // the slice is encoded and pushed out.
        void encode(void);

    private:
        SliceEncoder(   std::string name,
                        frame_buffer_t fb,
                        image_data_source_t dsrc,
                        uint32_t slice_nr);

        image_data_source_t     data_queue;
        frame_buffer_t          fb;
        uint32_t                slice_nr;
};
