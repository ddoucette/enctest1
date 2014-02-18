#pragma once
#include <memory>
#include <list>
#include <cstdint>
#include "FrameBuffer.h"


class ISurface;
typedef std::shared_ptr<ISurface> isurface_t;

class ISurfaceGroup
{
    public:
        ISurfaceGroup();
        ~ISurfaceGroup();
        std::list<isurface_t> get_surfaces(void) { return this->surfaces; };
    private:
        std::list<isurface_t> surfaces;
};

class ISurface
{
    public:
        static isurface_t Create(std::string name);

        // Blit the surface pixels into the provided frame buffer.
        virtual bool grab(FrameBuffer *fb)=0;

        // Get the dimension and topology for this surface.
        virtual bool set_topology(int32_t x, int32_t y)=0;
        virtual bool set_dimension(uint32_t width, uint32_t height, uint32_t bpp)=0;

        uint32_t get_width(void) { return this->width; };
        uint32_t get_height(void) { return this->height; };
        uint32_t get_bpp(void) { return this->bpp; };
        int32_t get_x(void) { return this->x; };
        int32_t get_y(void) { return this->y; };

    protected:
        // Width/height of the surface
        uint32_t    width, height;

        // Topology.  Position of this surface in relation
        // to it's neighbors, if it has neighbours.
        // Can be negative!
        int32_t     x, y;

        // Bits per pixel.
        uint32_t    bpp;
};
