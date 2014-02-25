#pragma once
#include <memory>
#include <list>
#include <cstdint>
#include <string>
#include "FrameBuffer.h"
#include "object.h"


class ISurface;
typedef std::shared_ptr<ISurface> isurface_t;

class ISurface : public Object
{
    public:
        ISurface(std::string name) : Object(name) {}
        ~ISurface() {}

        // Blit the surface pixels into the provided frame buffer.
        virtual bool grab(FrameBuffer *fb)=0;

        // Blit the framebuffer to the surface.  Client output.
        virtual bool put(FrameBuffer *fb)=0;

        std::string get_id(void) { return this->id; };
        uint32_t get_width(void) { return this->width; };
        uint32_t get_height(void) { return this->height; };
        uint32_t get_bpp(void) { return this->bpp; };
        int32_t get_x(void) { return this->x; };
        int32_t get_y(void) { return this->y; };

    protected:
        // Surface ID.  Unique string identifier
        std::string id;

        // Width/height of the surface
        uint32_t    width, height;

        // Topology.  Position of this surface in relation
        // to it's neighbors, if it has neighbours.
        // Can be negative!
        int32_t     x, y;

        // Bits per pixel.
        uint32_t    bpp;
};
