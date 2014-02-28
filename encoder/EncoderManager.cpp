#include "EncoderManager.h"
#include "SurfaceEncoder.h"
#include "logger.h"
#include "Threading.h"

static encoder_manager_t g_instance = NULL;

encoder_manager_t EncoderManager::GetInstance(void)
{
    if (g_instance == NULL)
        g_instance = encoder_manager_t(new EncoderManager("EncoderManager"));
    return g_instance;
}

void EncoderManager::Finalize(void)
{
    g_instance = NULL;
}

EncoderManager::~EncoderManager()
{
}

bool EncoderManager::add_security_configuration(security_configuration_t sec)
{
    std::lock_guard<std::mutex> lck(this->lock);
    sec_confs.push_back(sec);
    return true;
}

void EncoderManager::remove_security_configuration(security_configuration_t sec)
{
    std::lock_guard<std::mutex> lck(this->lock);
    sec_confs.remove(sec);
}

surface_binding_t EncoderManager::connect(
                                    isurface_t surface,
                                    protocol_connection_t prconn)
{
    std::lock_guard<std::mutex> lck(this->lock);
    // Ensure there is a valid security configuration for this connection
    security_configuration_t sec = find_security_configuration(prconn->get_cookie());
    if (sec == NULL)
    {
        mLOG_INFO("Connection (%s) has no valid security configuration!",
                  prconn->get_name().c_str());
        return NULL;
    }

    surface_binding_t sbind = SurfaceBinding::Create(surface, prconn);
    bindings.push_back(sbind);
    mLOG_INFO("Connecting client (%s) to surface (%s)",
              surface->get_name().c_str(), prconn->get_name().c_str());
    return sbind;
}

void EncoderManager::disconnect(surface_binding_t binding)
{
    std::lock_guard<std::mutex> lck(this->lock);
}

void EncoderManager::event_rcv(event_source_t esrc, event_t event)
{
}

security_configuration_t EncoderManager::find_security_configuration(cookie_t cookie)
{
    for (std::list<security_configuration_t>::iterator it = sec_confs.begin();
         it != sec_confs.end();
         it++)
    {
        security_configuration_t sec = *it;
        if (sec->get_cookie() == cookie)
            return sec;
    }
    return NULL;
}

EncoderManager::EncoderManager(std::string name) :
                                Object(name)
{
    thread_pool = ThreadPool::Create(10);
}
