#include <types.h>

class EncoderMaster
{
    public:
        ~EncoderMaster();

        start(void);
        stop(void);
        process_frame(void);

    private:
        EncoderMaster();
        uint8_t *frame0;
}
