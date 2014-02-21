#pragma once
#include "ISurface.h"
#include "ProtocolConnection.h"

class EncoderManager : public Object
{
    public:
        // Singleton
        static EncoderManager* GetInstance(void);
        static void Finalize(void);

        bool bind(ISurface *surf, ProtocolConnection *prconn);
        void unbind(ISurface *surf, ProtocolConnection *prconn);
    private:
        std::list<SurfaceEncoder*> surface_encoders;
};
