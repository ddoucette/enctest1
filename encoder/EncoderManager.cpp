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
    while (!bindings.empty())
    {
        surface_binding_t binding = bindings.front();
        bindings.pop_front();
        this->disconnect(binding);
    }
}

bool EncoderManager::add_security_configuration(security_configuration_t sec)
{
    std::lock_guard<std::mutex> lck(this->lock);
    sec_confs.push_back(sec);
    return true;
}

void EncoderManager::remove_security_configuration(security_configuration_t sec)
{
    std::list<surface_binding_t> dead_bindings;

    {
        std::lock_guard<std::mutex> lck(this->lock);
        sec_confs.remove(sec);

        // Find any/all bindings using this security configuration
        // and disconnect them.
        for (std::list<surface_binding_t>::iterator it = bindings.begin();
             it != bindings.end();
             it++)
        {
            surface_binding_t sbind = *it;
            if (sbind->get_security_configuration() == sec)
                dead_bindings.push_back(sbind);
        }
    }
    // Outside of lock context.  Lock is not re-entrant.
    while (!dead_bindings.empty())
    {
        this->disconnect(dead_bindings.front());
        dead_bindings.pop_front();
    }
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

    // Find or create a surface encoder for the surface requested.
    // There may already be an encoder for the specified surface, so
    // look first.
    // Once the encoder is available, plug in the protocol connection to the
    // encoder's data output source.
    surface_encoder_t surf_enc = find_encoder_by_surface(surface);
    if (surf_enc == NULL)
    {
        // No encoder, create one.
        try
        {
            surf_enc = SurfaceEncoder::Create(surface);
            // Connect up data source to data receiver
            surf_enc->get_data_source()->connect(prconn->get_image_input_channel());
        } catch (std::bad_alloc& exc) {
            mLOG_ERROR("Cannot allocate a surface encoder for connection (%s)",
                        prconn->get_name().c_str());
            return NULL;
        }
        surface_encoders.push_back(surf_enc);
        surf_enc->start(thread_pool);
    }
    mASSERT(surf_enc != NULL);
    mASSERT(surface != NULL);
    mASSERT(prconn != NULL);
    mASSERT(sec != NULL);
    surface_binding_t sbind = SurfaceBinding::Create(surface, prconn, sec, surf_enc);
    bindings.push_back(sbind);
    mLOG_INFO("Connecting client (%s) to surface (%s)",
              surface->get_name().c_str(), prconn->get_name().c_str());

    // XXX go through all the security authorizations and connect
    // up the other encoders.
    return sbind;
}

void EncoderManager::disconnect(surface_binding_t sbind)
{
    std::lock_guard<std::mutex> lck(this->lock);
    // start by removing the binding from our list.
    bindings.remove(sbind);

    // Disconnect the prconn receiver from the encoder output.
    protocol_connection_t prconn = sbind->get_protocol_connection();
    surface_encoder_t surf_enc = sbind->get_surface_encoder();
    surf_enc->get_data_source()->disconnect(prconn->get_image_input_channel());
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

surface_binding_t EncoderManager::find_binding_by_sec(security_configuration_t sec)
{
    for (std::list<surface_binding_t>::iterator it = bindings.begin();
         it != bindings.end();
         it++)
    {
        surface_binding_t sbind = *it;
        if (sbind->get_security_configuration() == sec)
            return sbind;
    }
    return NULL;
}

EncoderManager::EncoderManager(std::string name) :
                                Object(name)
{
    thread_pool = ThreadPool::Create(10);
}
