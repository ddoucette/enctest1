#pragma once

#define mCOMPILE_ASSERT(cond) \
    { static unsigned long __arr__[(cond)*2-1] = {0}; __arr__[0]++; }
