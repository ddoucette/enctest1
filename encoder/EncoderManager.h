#pragma once
#include "SecurityConfiguration.h"
#include "ISurface.h"
#include "Threading.h"

// The encoder manager is the top-level object for the core
// encoder stand-alone application.
// The encoder manager receives a security configuration + cookie
// from the host agent to determine authorized client connections
// to this.
// The core encoder application provides a socket interface to connecting
// clients to allow surfaces to be connected and transmitted.

class SurfaceEncoder;
typedef std::shared_ptr<SurfaceEncoder> surface_encoder_t;

class ProtocolConnection;
typedef std::shared_ptr<ProtocolConnection> protocol_connection_t;

class SurfaceBinding;
typedef std::shared_ptr<SurfaceBinding> surface_binding_t;

class SurfaceBinding
{
    public:
        static surface_binding_t Create(isurface_t surface,
                                        protocol_connection_t prconn,
                                        security_configuration_t sec,
                                        surface_encoder_t surf_enc)
        {
            return surface_binding_t(new SurfaceBinding(surface, prconn, sec, surf_enc));
        }

        isurface_t get_surface(void) { return surface; };
        protocol_connection_t get_protocol_connection(void) { return prconn; };
        security_configuration_t get_security_configuration(void) { return sec; };
        surface_encoder_t get_surface_encoder(void) { return surf_enc; };

        ~SurfaceBinding(){};

    private:
        SurfaceBinding( isurface_t surface,
                        protocol_connection_t prconn,
                        security_configuration_t sec,
                        surface_encoder_t surf_enc)
        {
            this->surface = surface;
            this->prconn = prconn;
            this->sec = sec;
            this->surf_enc = surf_enc;
        }
        isurface_t surface;
        protocol_connection_t prconn;
        security_configuration_t sec;
        surface_encoder_t surf_enc;
};

class EncoderManager;
typedef std::shared_ptr<EncoderManager> encoder_manager_t;

class EncoderManager :
            public Object,
            public EventReceiver,
            public EventSource
{
    public:
        enum
        {
            EVENT_DISCONNECT
        };

        // Singleton
        static encoder_manager_t GetInstance(void);
        static void Finalize(void);

        ~EncoderManager();

        bool add_security_configuration(security_configuration_t sec);
        void remove_security_configuration(security_configuration_t sec);

        bool is_authorized(cookie_t cookie);

        // (Dis)connect to the specified surface
        // Verifies necessary priviledges in the security_configuration_t.
        surface_binding_t connect(isurface_t surface, protocol_connection_t prconn);
        void disconnect(surface_binding_t binding);

        void event_rcv(event_source_t esrc, event_t event);

    private:
        EncoderManager(std::string name);

        std::mutex lock;

        // Security configurations.
        std::list<security_configuration_t> sec_confs;
        security_configuration_t find_security_configuration(cookie_t cookie);

        // Connection bindings
        std::list<surface_binding_t> bindings;

        // Surface encoders
        std::list<surface_encoder_t> surface_encoders;

        // Input encoders, audio encoders, plugin managers, ...
        // XXX tbd

        // Thread pool to use for all encoders.
        thread_pool_t thread_pool;
};
