#pragma once
#include <memory>
#include <list>
#include <map>
#include <mutex>
#include "SurfaceEncoder.h"
#include "ProtocolConnection.h"
#include "ISurfaceManager.h"


//
//  Application Manager
//  The application manager is the top-level object and the only
//  interface between the connection broker and the clients
//  connecting in, pre-authorization.
//
//  The application manager provides the connection broker with:
//    1) A list of surfaces the user can connect to:
//      a) State of each surface. Connected/disconnected.
//          i) If connected, user ID of the connection.
//      b) Available encoders.  Audio, plugins, ...
//      c) Specific information about the surface:
//          i) Application name, or desktop name.
//          ii) Icon/thumbnail.
//
//    2) Interface to accept security tokens from the connection broker
//      The application manager will receive tokens periodically
//      from the connection broker.  These tokens will be used to
//      verify requests from clients.  Client connections will contain
//      request parameters + nonce + hash.  The application manager will
//      confirm all requests using the token.
//
//  The application manager will periodically connect to the connection
//  broker and update it with it's inventory, if required.  In order to
//  minimize the amount of communication between the encoder host and
//  the CB, the connection broker will request an update from the host
//  if required.  If there are no actively logged in users for the host,
//  the CB may not request a full surface availability update.
//  We must be able to create a scalable system, which means long-lived
//  persistent connections are not possible.
//  All connections between the CB and this host are initiated by this
//  host.  This is necessary because of NAT/firewall considerations.

class ApplicationManager;
typedef std::shared_ptr<ApplicationManager> application_manager_t;
class ConnectionInfo;
typedef std::shared_ptr<ConnectionInfo> connection_info_t;

class ConnectionInfo
{
    // This class provides a container for all information required
    // to establish a connection from a client.
    // This is used by the connection broker in response to a
    // connection request from the user from within the connection
    // broker.
    // This structure contains the surface to connect to along with
    // a security token which must be presented by the client
    // for connection establishment.
    // This structure must also contain the list of encoders the
    // client is authorized to connect to, along with given access
    // privilidges for each.
    // For example:
    //     Audio Output: True
    //     Audio Input:  False
    //     Keyboard Input: True
    //     Mouse Input: True
    //     Plugin<X>: True
    public:
        ~ConnectionInfo();

        static connection_info_t Create(void);

    private:
        ConnectionInfo();

        // Number of seconds this connection should remain active
        uint32_t ttl;

        // String identifier for the surface to connect
        std::string surface_id;

        // String token for security.
        std::string security_token;

        // Map of all connection specifiers.  See comment above.
        std::map<std::string,bool> params;
};


class ApplicationManager :
            public Object,
            public EventReceiver,
            public EventSource
{
    public:
        typedef enum application_manager_events
        {
            EVENT_SHUTDOWN
        } event_t;

        // Singleton
        static application_manager_t GetInstance(void);

        // Clean up singleton and close down.
        static void Finalize(void);

        // Return the list of surfaces available.
        // Provides the connection broker with a list of
        // available surfaces.
        std::list<isurface_t> get_surfaces(void);

        // Create/reserve a connection.
        // This is used by the connection broker to create a connection
        // reservation for a client.  This request contains the necessary
        // security tokens for the connection.
        // The connection info also contains permissions for the connection
        // which can be updated any time during the connection.
        bool connection_reserve(connection_info_t cinfo);
        bool connection_update(connection_info_t cinfo);
        void connection_close(connection_info_t cinfo);

        // Provides a list of current connections
        std::list<connection_info_t> get_connections(void);

        // (Dis)connect to the specified surface
        surface_binding_t connect(isurface_t surface, protocol_connection_t prconn);
        void disconnect(surface_binding_t binding);

        void event_rcv(event_source_t esrc, event_t event);

        ~ApplicationManager();
    private:
        ApplicationManager();

        std::mutex lock;

        // Connection bindings
        std::list<surface_binding_t> bindings;

        // Surface encoders
        std::list<surface_encoder_t> surface_encoders;
        // Input encoders, audio encoders, plugin managers, ...
};
