#pragma once
#include <memory>
#include "ImageTile.h"
#include "DataSource.h"

typedef std::shared_ptr<DataSource<ImageTile>> tile_data_source_t;
