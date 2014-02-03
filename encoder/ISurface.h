#pragma once
#include <cstdint>
#include "FrameBuffer.h"

struct surface_info
{
    // Width/height of the surface
    uint32_t    width, height;

    // Position of this surface in relation to the
    // group of surfaces.
    uint32_t    x, y;
} surface_info_t;

class ISurfaceGroup
{
    public:
        ISurfaceGroup();
        ~ISurfaceGroup();
        std::list<ISurface> get_surfaces(void) { return this->surfaces; };
    private:
        std::list<ISurface> surfaces;
};

class ISurface
{
    public:
        // Blit the surface pixels into the provided frame buffer.
        virtual bool grab(FrameBuffer *fb)=0;

        // Get the dimension and topology for this surface.
        virtual bool set_topology(int32_t x, int32_t y)=0;
        virtual bool set_dimension(uint32_t width, uint32_t height)=0;

        uint32_t get_width(void) { return this->width; };
        uint32_t get_height(void) { return this->height; };
        uint32_t get_bpp(void) { return this->bpp; };
        int32_t get_x(void) { return this->x; };
        int32_t get_y(void) { return this->y; };

    private:
        // Width/height of the surface
        uint32_t    width, height;

        // Topology.  Position of this surface in relation
        // to it's neighbors, if it has neighbours.
        // Can be negative!
        int32_t     x, y;

        // Bits per pixel.
        uint32_t    bpp;
};
