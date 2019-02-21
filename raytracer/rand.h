#include <random>

thread_local extern unsigned int g_seed;
thread_local extern std::ranlux24_base base;
thread_local extern std::uniform_real_distribution<float> urd;
 
//Used to seed the generator.
inline void fastrand_seed( int seed )
{
    g_seed = seed;
	base.seed(seed);
}
//fastrand routine returns one integer, similar output value range as C lib.
inline int fastrand()
{
    g_seed = (214013*g_seed+2531011);
    return (g_seed>>16)&0x7FFF;
}

inline float fastrandF() {
    return urd(base);
}
