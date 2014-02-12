#pragma once
#include <cassert>

#define mASSERT(cond) assert(cond)

#ifndef IDEBUG
#define mASSERT_DEBUG(cond)
#else
#define mASSERT_DEBUG(cond) mASSERT(cond)
#endif
