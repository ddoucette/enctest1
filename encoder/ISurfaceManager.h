#pragma once
#include <list>
#include "ISurface.h"

class ISurfaceManager : public Object, Notifier
{
    public:
        // Singleton
        static ISurfaceManager* GetInstance(void);

        // Get a list of all surfaces.
        std::list<ISurfaceGroup*>& get_surfaces(void);

    private:
        std::list<ISurfaceGroup*> surfaces;
};
