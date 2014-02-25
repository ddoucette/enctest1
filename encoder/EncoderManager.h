#pragma once
#include "SecurityConfiguration.h"
#include "ProtocolConnection.h"
#include "ISurface.h"
#include "SurfaceEncoder.h"

// The encoder manager is the top-level object for the core
// encoder stand-alone application.
// The encoder manager receives a security configuration + cookie
// from the host agent to determine authorized client connections
// to this.
// The core encoder application provides a socket interface to connecting
// clients to allow surfaces to be connected and transmitted.

class SurfaceBinding;
typedef std::shared_ptr<SurfaceBinding> surface_binding_t;

class SurfaceBinding
{
    public:
        static surface_binding_t Create(isurface_t surface, protocol_connection_t prconn)
        {
            return surface_binding_t(new SurfaceBinding(surface, prconn));
        }

        isurface_t get_surface(void) { return surface; };
        protocol_connection_t get_protocol_connection(void) { return prconn; };

        ~SurfaceBinding(){};

    private:
        SurfaceBinding(isurface_t surface, protocol_connection_t prconn)
        {
            this->surface = surface;
            this->prconn = prconn;
        }
        isurface_t surface;
        protocol_connection_t prconn;
};

class EncoderManager;
typedef std::shared_ptr<EncoderManager> encoder_manager_t;

class EncoderManager :
            public Object,
            public EventReceiver,
            public EventSource
{
    public:
        // Singleton
        static encoder_manager_t GetInstance(void);
        static void Finalize(void);

        ~EncoderManager();

        bool set_security_configuration(security_configuration_t sec,
                                        cookie_t cookie);
        cookie_t get_cookie(void);

        // (Dis)connect to the specified surface
        // Verifies necessary priviledges in the security_configuration_t.
        surface_binding_t connect(isurface_t surface, protocol_connection_t prconn);
        void disconnect(surface_binding_t binding);

        void event_rcv(event_source_t esrc, event_t event);

    private:
        EncoderManager(std::string name);

        std::mutex lock;
        // Connection bindings
        std::list<surface_binding_t> bindings;
        // Surface encoders
        std::list<surface_encoder_t> surface_encoders;
        // Input encoders, audio encoders, plugin managers, ...

        security_configuration_t    sec;
        cookie_t                    cookie;
};
