#pragma once
#include <memory>
#include "ISurfaceManager.h"
#include "ISurfaceTest.h"
#include "ISurface.h"

class ISurfaceManagerTest;
typedef std::shared_ptr<ISurfaceManagerTest> isurface_manager_test_t;

class ISurfaceManagerTest : public ISurfaceManager
{
    public:
        static isurface_manager_test_t GetInstance(void);
        static void Finalize(void);

        void create_surface(uint32_t width,
                            uint32_t height,
                            int32_t x,
                            int32_t y,
                            uint32_t bpp)
        {
            isurface_t surface = ISurfaceTest::Create(width, height, x, y, bpp);
            surfaces.push_back(surface);
        }

        std::list<isurface_t> get_surfaces(void)
        {
            return surfaces;
        }

        ~ISurfaceManagerTest()
        {
            while (!surfaces.empty())
                surfaces.pop_front();
        };
    private:
        ISurfaceManagerTest() {};
        std::list<isurface_t> surfaces;
};

static isurface_manager_test_t g_instance;

isurface_manager_test_t ISurfaceManagerTest::GetInstance(void)
{
    if (g_instance == NULL)
    {
        g_instance = isurface_manager_test_t(new ISurfaceManagerTest());
    }
    return g_instance;
}

void ISurfaceManagerTest::Finalize(void)
{
    g_instance = NULL;
}
