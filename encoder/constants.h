#pragma once

#define MAX_WIDTH           4096
#define MAX_HEIGHT          4096
#define MAX_STRIDE          (MAX_WIDTH * 4)
#define SLICE_HEIGHT        32
#define CACHELINE_BYTES     64

#define mALIGN(x, align)    \
        (((x) + ((align)-1)) & ~((align)-1))
