#pragma once
#include <memory>
#include "object.h"
#include "DataReceiver.h"
#include "ImageTile.h"

class ProtocolConnection;
typedef std::shared_ptr<ProtocolConnection> protocol_connection_t;

class ProtocolConnection : public Object
{
    public:
        static protocol_connection_t Create(std::string client_info);

        ~ProtocolConnection();

        data_receiver_t<ImageTile> get_input_channel(void);

    private:
        ProtocolConnection();
        data_receiver_t<ImageTile> tile_input;
};
