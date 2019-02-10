#include "rand.h"
#include "types.h"
thread_local U32 g_seed;

thread_local std::ranlux24_base base;
thread_local std::uniform_real_distribution<float> urd(0.0, 1.0);
