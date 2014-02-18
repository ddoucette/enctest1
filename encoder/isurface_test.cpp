#include "ISurface.h"

class ISurfaceMock : public ISurface, public Object
{
    public:
        ISurfaceMock(std::string name) : Object(name) {}
        ~ISurfaceMock() {}

        // Blit the surface pixels into the provided frame buffer.
        bool grab(FrameBuffer *fb)
        {
            return true;
        }

        // Get the dimension and topology for this surface.
        bool set_topology(int32_t x, int32_t y)
        {
            this->x = x;
            this->y = y;
            return true;
        }

        bool set_dimension(uint32_t width, uint32_t height, uint32_t bpp)
        {
            this->width = width;
            this->height = height;
            this->bpp = bpp;
            return true;
        }
};

isurface_t ISurface::Create(std::string name)
{
    isurface_t isurf(new ISurfaceMock(name));
    return isurf;
}
