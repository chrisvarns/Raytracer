#include <random>

extern unsigned int g_seed;
 
//Used to seed the generator.
inline void fast_srand( int seed )
{
    g_seed = seed;
}
//fastrand routine returns one integer, similar output value range as C lib.
inline int fastrand()
{
    g_seed = (214013*g_seed+2531011);
    return (g_seed>>16)&0x7FFF;
}

float fastrandF();
