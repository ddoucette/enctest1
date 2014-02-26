#include "EncoderManager.h"
#include "SurfaceEncoder.h"
#include "logger.h"

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

bool EncoderManager::set_security_configuration(
                                        security_configuration_t sec,
                                        cookie_t cookie)
{
    this->sec = sec;
    this->cookie = cookie;
    return true;
}

cookie_t EncoderManager::get_cookie(void)
{
    return this->cookie;
}

surface_binding_t EncoderManager::connect(
                                    isurface_t surface,
                                    protocol_connection_t prconn)
{
    surface_binding_t sbind = SurfaceBinding::Create(surface, prconn);
    bindings.push_back(sbind);
    mLOG_INFO("Connecting client (%s) to surface (%s)",
              surface->get_name().c_str(), prconn->get_name().c_str());
    return sbind;
}

void EncoderManager::disconnect(surface_binding_t binding)
{
}

void EncoderManager::event_rcv(event_source_t esrc, event_t event)
{
}

EncoderManager::EncoderManager(std::string name) :
                                Object(name)
{
    cookie = INVALID_COOKIE;
    thread_pool = Threading::Create(10);
}
