#include <memory>
#include "SurfaceEncoder.h"
#include "ISurface.h"
#include "FrameBuffer.h"

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

    this->data_queue = DataSource<ImageTile>::Create("name");
    for (uint32_t i = 0; i < fb->get_nr_slices(); i++)
    {
        slice_encoder_t slice_enc = SliceEncoder::Create(this->shared_from_this(),
                                                         fb, data_queue, i);
        slice_encoders.push_back(slice_enc);
    }
}

SurfaceEncoder::~SurfaceEncoder()
{
    while (!slice_encoders.empty())
        slice_encoders.pop_front();

    this->surface = NULL;
    this->fb = NULL;
    this->data_queue = NULL;
}

void SurfaceEncoder::run(void)
{
}

void SurfaceEncoder::event_rcv(event_source_t src, event_t event)
{
}
