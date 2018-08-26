#include "perlin.h"

inline float trilinear_interp(float c[2][2][2], float u, float v, float w) {
    float accum = 0;
    for(int i = 0; i < 2; i++) {
        for(int j = 0; j < 2; j++) {
            for(int k = 0; k < 2; k++) {
                accum += (i*u + (1-i)*(1-u)) *
                         (j*v + (1-j)*(1-v)) *
                         (k*w + (1-k)*(1-w)) * c[i][j][k];
            }
        }
    }
    return accum;
}

float perlin::noise(const vec3& p) const {
    int i = floor(p.x);
    int j = floor(p.y);
    int k = floor(p.z);
    float u = p.x - i;
    float v = p.y - j;
    float w = p.z - k;
    float c[2][2][2];
    for(int di = 0; di < 2; di++) {
        for(int dj = 0; dj < 2; dj++) {
            for(int dk = 0; dk < 2; dk++) {
                c[di][dj][dk] = ranfloat[perm_x[(i+di) & 255] ^ perm_y[(j+dj) & 255] ^ perm_z[(k+dk) & 255]];
            }
        }
    }
    return trilinear_interp(c, u, v, w);
}

static float* perlin_generate() {
    float* p = new float[256];
    for(int i = 0; i < 256; i++) {
        p[i] = drand48();
    }
    return p;
}

void permute(int* p, int n) {
    for(int i = n-1; i > 0; i--) {
        int target = int(drand48()*(i+1));
        std::swap(p[i], p[target]);
    }
}

static int* perlin_generate_perm() {
    int* p = new int[256];
    for(int i = 0; i < 256; i++) {
        p[i] = i;
    }
    permute(p, 256);
    return p;
}

perlin::perlin() {

    static bool generated = false;

    if(generated) return;

    generated = true;
    ranfloat = perlin_generate();
    perm_x = perlin_generate_perm();
    perm_y = perlin_generate_perm();
    perm_z = perlin_generate_perm();
}

float* perlin::ranfloat;
int* perlin::perm_x;
int* perlin::perm_y;
int* perlin::perm_z;
