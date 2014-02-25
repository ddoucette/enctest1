#pragma once
#include "ISurface.h"

// Mock surface for testing.

class ISurfaceTest;
typedef std::shared_ptr<ISurfaceTest> isurface_test_t;

class ISurfaceTest : public ISurface
{
    public:
        static isurface_t Create(   uint32_t width,
                                    uint32_t height,
                                    int32_t x,
                                    int32_t y,
                                    uint32_t bpp)
        {
            std::string surface_name = std::to_string(width) +
                                       "x" + std::to_string(height);
            return isurface_t(new ISurfaceTest(surface_name, width, height, x, y, bpp));
        }

        bool grab(FrameBuffer *fb)
        {
            return true;
        }

        bool put(FrameBuffer *fb)
        {
            return true;
        }

        ~ISurfaceTest(){};

    private:
        ISurfaceTest(   std::string name,
                        uint32_t width,
                        uint32_t height,
                        int32_t x,
                        int32_t y,
                        uint32_t bpp) :
                        ISurface(name)
        {
            this->width = width;
            this->height = height;
            this->bpp = bpp;
            this->x = x;
            this->y = y;
        }
};
