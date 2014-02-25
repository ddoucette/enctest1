#include <string>
#include "SliceEncoder.h"
#include "ImageTile.h"

slice_encoder_t SliceEncoder::Create(
                                    surface_encoder_t surface_encoder,
                                    frame_buffer_t fb,
                                    image_tile_source_t dsrc,
                                    uint32_t slice_nr)
{
    std::string slice_name = std::string("slice-" + std::to_string(slice_nr));
    slice_encoder_t slice_enc(new SliceEncoder( slice_name,
                                                surface_encoder,
                                                fb,
                                                dsrc,
                                                slice_nr));
    return slice_enc;
}

SliceEncoder::SliceEncoder( std::string name,
                            surface_encoder_t surface_encoder,
                            frame_buffer_t fb,
                            image_tile_source_t data_queue,
                            uint32_t slice_nr) :
                            Object(name)
{
    this->data_queue = data_queue;
    this->slice_nr = slice_nr;
    this->fb = fb;
}

SliceEncoder::~SliceEncoder()
{
}

void SliceEncoder::encode(void)
{
    trigger_event(EVENT_ENCODE_COMPLETE);
}

void SliceEncoder::run(void)
{
}
