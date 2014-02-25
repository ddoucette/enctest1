#include "ApplicationManager.h"
#include "logger.h"

static application_manager_t g_instance = NULL;

application_manager_t ApplicationManager::GetInstance(void)
{
    if (g_instance == NULL)
        g_instance = application_manager_t(new ApplicationManager());
    return g_instance;
}

void ApplicationManager::Finalize(void)
{
    g_instance = NULL;
}

std::list<isurface_t> ApplicationManager::get_surfaces(void)
{
    return ISurfaceManager::GetInstance()->get_surfaces();
}

bool ApplicationManager::connection_reserve(connection_info_t cinfo)
{
    std::lock_guard<std::mutex> lck(this->lock);
    // Find the surface specified.
    isurface_t surface = ISurfaceManager::GetInstance()->find(cinfo->get_surface_id());
    if (surface == NULL)
    {
        mLOG_INFO("Invalid surface ID specified on connection reserve. (%S)",
                  cinfo->get_surface_id());
        return false;
    }

    if (cinfo->get_security_token() == "")
    {
        mLOG_INFO("Invalid/empty security token specified.");
        return false;
    }

    mLOG_INFO("Received connection: surface ID (%S), token (%S)",
              cinfo->get_surface_id(), cinfo->get_security_token());
    connections.push_back(cinfo);
}

bool ApplicationManager::connection_update(connection_info_t cinfo)
{
    std::lock_guard<std::mutex> lck(this->lock);
    connection_info_t cinfo_orig = find_connection(cinfo->get_surface_id(),
                                                   cinfo->get_security_token());
    if (cinfo_orig == NULL)
    {
        mLOG_INFO("Cannot update specified connection (%S:%S)",
                  cinfo->get_surface_id(), cinfo->get_security_token());
        return false;
    }
    connections.remove(cinfo_orig);
    connections.push_back(cinfo);
    return true;
}

void ApplicationManager::connection_close(connection_info_t cinfo)
{
    std::lock_guard<std::mutex> lck(this->lock);
    connection_info_t cinfo_orig = find_connection(cinfo->get_surface_id(),
                                                   cinfo->get_security_token());
    if (cinfo_orig == NULL)
    {
        mLOG_INFO("Cannot close specified connection (%S:%S)",
                  cinfo->get_surface_id(), cinfo->get_security_token());
        return false;
    }
    connections.remove(cinfo_orig);
    return true;
}

surface_binding_t ApplicationManager::connect(  isurface_t surface,
                                                protocol_connection_t prconn)
{
    surface_binding_t sbind = surface_binding_t(new SurfaceBinding(surface, prconn));
    bindings.push_back(sbind);
    mLOG_INFO("Connecting client (%s) to surface (%s)",
              surface->get_name(), prconn->get_name());
    return sbind;
}

void ApplicationManager::disconnect(surface_binding_t binding)
{
}

void ApplicationManager::event_rcv(event_source_t esrc, event_t event)
{
}

ApplicationManager::~ApplicationManager()
{
    while (!bindings.empty())
        bindings.pop_front();
    while (!surface_encoders.empty())
        surface_encoders.pop_front();
}

ApplicationManager::ApplicationManager()
{
}
