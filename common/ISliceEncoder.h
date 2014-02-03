/*
 * <Interface Name>
 *
 * Copyright Dan Doucette (c) 2014.
 */

#include <itypes.h>

#define SLICE_HEIGHT    (32)

class ISliceEncoder
{
    public:
        ISliceEncoder(uint8 *slice_addr, uint32 stride);
        ~ISliceEncoder();


};
