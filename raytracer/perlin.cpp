#include "perlin.h"

float perlin::noise(const vec3& p) const {
    float u = p.x - floor(p.x);
    float v = p.y - floor(p.y);
    float w = p.z - floor(p.z);
    int i = int(4*p.x) & 255;
    int j = int(4*p.y) & 255;
    int k = int(4*p.z) & 255;
    return ranfloat[perm_x[i] ^ perm_y[j] ^ perm_z[k]];
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
