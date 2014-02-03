#include <sys/types.h>
#include <string>
#include <vector>
#include <memory>

#include "ISurface.h"

struct surface_encoder_stats
{
    uint64_t    nr_frames;
    uint64_t    nr_tiles;
    uint64_t    nr_input_bytes;
    uint64_t    nr_output_bytes;
};

class SurfaceEncoder
{
    public:
        SurfaceEncoder(std::string name);
        ~SurfaceEncoder();

        bool configure( shared_ptr<ISurfaceDescriptor> sdesc,
                        std::vector<shared_ptr<TileQueue>> output_queues);
        bool encode_frame(void);
        void get_stats(struct surface_encoder_stats *stats);

    private:
        shared_ptr<ISurface> s0;
        shared_ptr<ISurface> s1;
        shared_ptr<ISurfaceDescriptor>  sdesc;
        vector<shared_ptr<TileQueue>>  *queues;
        struct surface_encoder_stats    stats;
};
