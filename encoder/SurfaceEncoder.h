#pragma once
#include "ISurface.h"
#include "SliceEncoder.h"

class SurfaceEncoder : public Object
{
    public:
        SurfaceEncoder(std::name, ISurface *surface);
        ~SurfaceEncoder();

    private:
        ISurface *surface;
        std::list<SliceEncoder*> slice_encoders;
};
