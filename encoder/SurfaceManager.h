#pragma once
#include <memory>

class ISurfaceManager;
typedef std::shared_ptr<ISurfaceManager> isurface_manager_t;

class ISurfaceManager : public Object
{
    public:
        static isurface_manager_t GetInstance(void);

        // Return a list of all known surface groups.
        std::list<isurface_group_t> get_surface_groups(void)
        {
            return surface_groups;
        }

        isurface_t find_surface(std::string surface_name)
        {
            std::lock_guard<std::mutex> lck(this->lock);
            for (std::list<isurface_group_t>::iterator it = surface_groups.begin();
                 it != surface_groups.end();
                 it++)
            {
                std::list<isurface_t> surfaces = *it->get_surfaces();
                for (std::list<isurface_t>::iterator sit = surfaces.begin();
                     sit != surfaces.end();
                     sit++)
                {
                    isurface_t surface = *it;
                    if (surface->get_name().compare(surface_name) == 0)
                        return surface;
                }
            }
            return NULL;
        }

        ~ISurfaceManager()
        {
            while (!surface_groups.empty())
                surface_groups.pop_front();
        }

    private:
        ISurfaceManager(std::string name, object_t parent) :
                        Object(name, parent) {}

        std::list<isurface_group_t> surface_groups;
}
