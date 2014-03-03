#pragma once
#include <memory>
#include "object.h"
#include "DataReceiver.h"
#include "ImageTile.h"
#include "SecurityConfiguration.h"
#include "EncoderManager.h"

class ProtocolConnection;
typedef std::shared_ptr<ProtocolConnection> protocol_connection_t;

class ProtocolConnection :
                    public Object,
                    public EventReceiver
{
    public:
        static protocol_connection_t Create(std::string name)
        {
            return protocol_connection_t(new ProtocolConnection(name));
        }

        ~ProtocolConnection() {}

        // Received events
        //   - EncoderManager -- DISCONNECT
        void event_rcv(event_source_t esrc, event_t event)
        {
            mLOG_INFO("Received event (%d)", event);
        }

        void connect(cookie_t cookie) { this->cookie = cookie; }
        cookie_t get_cookie(void) { return this->cookie; }

        image_tile_receiver_t get_image_input_channel(void)
        {
            return tile_input;
        }

    private:
        ProtocolConnection(std::string name) : Object(name) {}
        image_tile_receiver_t tile_input;
        cookie_t cookie;
};
