#include "ISurfaceManager.h"
#include "ISurfaceManagerTest.h"


static isurface_manager_test_t g_instance;

isurface_manager_t ISurfaceManager::GetInstance(void)
{
    if (g_instance == NULL)
    {
        g_instance = isurface_manager_test_t(new ISurfaceManagerTest());
    }
    return g_instance;
}

void ISurfaceManager::Finalize(void)
{
    g_instance = NULL;
}
