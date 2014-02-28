#pragma once
#include <list>
#include <memory>
#include "ISurface.h"

class ISurfaceManager;
typedef std::shared_ptr<ISurfaceManager> isurface_manager_t;

class ISurfaceManager :
            public Object
{
    public:
        // Singleton
        static isurface_manager_t GetInstance(void);
        static void Finalize(void);

        // Get a list of all surfaces.
        virtual std::list<isurface_t>& get_surfaces(void)=0;

        ~ISurfaceManager() {};
    protected:
        ISurfaceManager() : Object("surface-manager") {}
};
