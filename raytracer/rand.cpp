#include "rand.h"
unsigned int g_seed;

float fastrandF() {
    static std::ranlux24_base base;
    static std::uniform_real_distribution<float> urd(0.0, 1.0);

    return urd(base);
}
