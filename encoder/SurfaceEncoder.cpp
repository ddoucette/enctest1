#include <memory>
#include "SurfaceEncoder.h"
#include "ISurface.h"
#include "FrameBuffer.h"


#define mHZ_TO_PERIOD_USEC(hz) (1000000/(hz))


surface_encoder_t SurfaceEncoder::Create(isurface_t surface)
{
    std::string encoder_name = std::string("encoder-" + surface->get_name());
    surface_encoder_t enc(new SurfaceEncoder(encoder_name, surface));
    return enc;
}

SurfaceEncoder::SurfaceEncoder( std::string name,
                                isurface_t surface) :
                                Object(name)
{
    this->surface = surface;
    uint32_t stride = surface->get_width() * (surface->get_bpp()/8);
    stride = mALIGN(stride, CACHELINE_BYTES);

    this->fb = FrameBuffer::Create(surface->get_width(),
                               surface->get_height(),
                               surface->get_bpp(),
                               stride);

    this->data_queue = DataSource<ImageTile>::Create(this->get_name());
    for (uint32_t i = 0; i < fb->get_nr_slices(); i++)
    {
        slice_encoder_t slice_enc = SliceEncoder::Create(this->shared_from_this(),
                                                         fb, data_queue, i);
        slice_encoders.push_back(slice_enc);
    }
    slice_ready_count = 0;
    usec_interval = mHZ_TO_PERIOD_USEC(30);
}

SurfaceEncoder::~SurfaceEncoder()
{
    while (!slice_encoders.empty())
        slice_encoders.pop_front();

    this->surface = NULL;
    this->fb = NULL;
    this->data_queue = NULL;
    this->thread_pool = NULL;
}

bool set_frequency(uint32_t hz)
{
    if (hz == 0 || hz > 60)
    {
        mLOG_ERROR("Invalid frequency specified (%d)", hz);
        return false;
    }
    usec_interval = mHZ_TO_PERIOD_USEC(hz);
    return true;
}

void SurfaceEncoder::run(void)
{
    std::list<slice_encoder_t> slices;
    std::lock_guard<std::mutex> lck(this->lock);
    for ( std::list<slice_encoder_t>::iterator it = slice_encoders->begin();
          it != slice_encoders->end();
          it++ )
    {
        slice_encoder_t slice = *it;
        if (!slice->is_ready())
        {
            mLOG_ERROR("Strange!  Slice (%s) is not ready for encoding!",
                       slice->get_name().c_str());
            // Reschedule this.
            // We may need to consider a full re-sync with the slice_ready_count
            // and the encode-done events from the slices.
            thread_pool->schedule(this->shared_from_this(), usec_interval);
            return;
        }
        slices.push_back(slice);
    }
    // We must schedule all slices, or no slices.  Hence the list.
    // Dont worry about the slices not being under lock, they can
    // never move from ready to scheduled other than right here.
    while (!slices.empty())
    {
        thread->schedule(slices.front());
        slices.pop_front();
    }
}

void SurfaceEncoder::start(thread_pool_t thread_pool)
{
    this->thread_pool = thread_pool;
    this->thread_pool->schedule(this->shared_from_this(), usec_interval);
}

void SurfaceEncoder::event_rcv(event_source_t src, event_t event)
{
    std::lock_guard<std::mutex> lck(this->lock);
    switch (event)
    {
        case SliceEncoder::EVENT_ENCODE_COMPLETE:
            slice_ready_count++;
            break;
        default:
            mLOG_ERROR("Invalid event received (%d)", event);
            mBUG();
    }

    if (slice_ready_count == slices.size())
    {
        slice_ready_count = 0;
        thread_pool->schedule(this->shared_from_this(), usec_interval);
    }
}
