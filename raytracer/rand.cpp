#include "rand.h"
unsigned int g_seed;

std::ranlux24_base base;
std::uniform_real_distribution<float> urd(0.0, 1.0);
