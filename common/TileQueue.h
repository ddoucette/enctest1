#pragma once
#include <memory>

class TileQueue : public std::enable_shared_from_this<TileQueue>
{
    public:
        static shared_ptr<TileQueue> Create(void);

    private:
        TileQueue();
        ~TileQueue();
};
