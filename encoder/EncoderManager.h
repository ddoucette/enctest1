#pragma once

class EncoderManager : public Object
{
    public:
        // Singleton
        static EncoderManager* GetInstance(void);

        bool bind(ISurface *surf, RemoteConnection *rconn);
        void unbind(ISurface *surf, RemoteConnection *rconn);
    private:
        std::list<SurfaceEncoder*> surface_encoders;
};
