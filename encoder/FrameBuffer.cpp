
#include "FrameBuffer.h"
#include <cstdint>
#include <cassert>
#include <memory>
#include "memory.h"
#include "logger.h"


FrameBuffer::FrameBuffer(   uint32_t width,
                            uint32_t height,
                            uint32_t bpp,
                            uint32_t stride)
{
    assert(width <= MAX_WIDTH);
    assert(width > 0);
    assert(height <= MAX_HEIGHT);
    assert(height >= SLICE_HEIGHT);
    assert(stride <= MAX_STRIDE);
    assert(bpp == 8 || bpp == 16 || bpp == 24 || bpp == 32);

    // Stride should be at least (Bpp * width)
    assert(stride >= (width * (bpp/8)));

    this->width = width;
    this->height = height;
    this->stride = stride;
    this->bpp = bpp;

    this->partial_slice = false;
    this->nr_slices = height / SLICE_HEIGHT;
    assert(this->nr_slices >= 1);
    // Check for a partial slice
    if (this->height % SLICE_HEIGHT != 0)
    {
        this->nr_slices++;
        this->partial_slice = true;
    }
    this->buffer = Memory::Malloc(  "framebuffer",
                                    stride * height * (bpp/8));
}

FrameBuffer::~FrameBuffer(void)
{
    Memory::Free(this->buffer);
    this->buffer = NULL;
}

frame_buffer_t
FrameBuffer::Create(uint32_t width,
                    uint32_t height,
                    uint32_t bpp,
                    uint32_t stride)
{
    frame_buffer_t fb(new FrameBuffer(width, height, bpp, stride));
    return fb;
}
