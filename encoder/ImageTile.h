#pragma once
#include "dbuf.h"
#include "DataSource.h"
#include "DataReceiver.h"

class ImageTile : public DBuf
{
    public:
        ImageTile(){};
        ~ImageTile(){};
};

typedef std::shared_ptr<DataReceiver<ImageTile>> image_tile_receiver_t;
typedef std::shared_ptr<DataSource<ImageTile>> image_tile_source_t;
