#pragma once

#include <cstdint>
#include <memory>
#include "iassert.h"
#include "constants.h"
#include "object.h"

class FrameBuffer;
typedef std::shared_ptr<FrameBuffer> frame_buffer_t;

class FrameBuffer
{
    public:
        ~FrameBuffer();
        static frame_buffer_t
            Create(uint32_t width,
                   uint32_t height,
                   uint32_t bpp,
                   uint32_t stride);

        void* get_slice(uint32_t slice_num)
        {
            mASSERT(slice_num < nr_slices);
            return (void*)((uintptr_t)buffer +
                    (slice_num * (SLICE_HEIGHT * stride)));
        };

        uint32_t get_width(void) { return this->width; };
        uint32_t get_height(void) { return this->height; };
        uint32_t get_bpp(void) { return this->bpp; };
        uint32_t get_stride(void) { return this->stride; };
        uint32_t get_nr_slices(void) { return this->nr_slices; };

    private:
        FrameBuffer(uint32_t width,
                    uint32_t height,
                    uint32_t bpp,
                    uint32_t stride);

        // Width/height of the surface
        uint32_t    width, height;

        // Number of slices in height.  Convenience.
        uint32_t    nr_slices;

        // True, if the height is not on a slice boundary.
        bool        partial_slice;

        // Bits per pixel.
        uint32_t    bpp;

        // Stride.  Number of bytes in a pixel row.
        uint32_t    stride;

        // Memory backing the frame buffer.
        void       *buffer;
};
