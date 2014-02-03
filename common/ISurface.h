#include <list>
#include <memory>

class ISurfaceDescriptor : public std::enable_shared_from_this<ISurfaceDescriptor>
{
    public:
        static std::list<shared_ptr<ISurfaceDescriptor>> GetAllSurfaces();

    private:
        ISurfaceDescriptor();
        ~ISurfaceDescriptor();
};

class ISurface : public std::enable_shared_from_this<ISurface>
{
    public:
        shared_ptr<ISurface> CreateSurface(shared_ptr<ISurfaceDescriptor> sdesc);

    private:
        ISurface();
        ~ISurface();
};
