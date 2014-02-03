#include "SurfaceEncoder.h"
#include "ISurface.h"
#include "TileQueue.h"
#include "logging.h"

SurfaceEncoder::SurfaceEncoder(std::string name)
{
    this->name = name;
    memset(this->stats, 0, sizeof(struct surface_encoder_stats));
    this->s0 = this->s1 = NULL;
    this->sdesc = NULL;
}

SurfaceEncoder::~SurfaceEncoder()
{
    this->s0 = NULL;
    this->s1 = NULL;
    this->sdesc = NULL;
}

bool SurfaceEncoder::configure(
                        shared_ptr<ISurfaceDescriptor> sdesc,
                        std::vector<shared_ptr<TileQueue>> *output_queues)
{
    this->queues = output_queues;
    this->sdesc = sdesc;
    this->s0 = ISurface->CreateInstance(sdesc)
    this->s1 = ISurface->CreateInstance(sdesc)
    if (this->s0 == NULL || this->s1 == NULL)
    {
        mLOG_ERROR("Cannot create all surfaces: s0(%p), s1(%p)",
                    this->s0, this->s1);
        return false;
    }
    return true;
}

bool SurfaceEncoder::encode_frame(void)
{
    this->stats.nr_frames++;
    return true;
}

void SurfaceEncoder::get_stats(struct surface_encoder_stats *stats)
{
    memcpy(stats, &this->stats, sizeof(struct surface_encoder_stats));
}
