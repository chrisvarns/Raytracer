#include "perlin.h"

float perlin::turb(vec3 p, int depth) const {
    float accum = 0;
    float weight = 1.0f;
    for(int i = 0; i < depth; i++) {
        accum += weight * noise(p);
        weight *= 0.5f;
        p *= 2;
    }
    return fabs(accum);
}

inline float perlin_interp(vec3 c[2][2][2], float u, float v, float w) {
    float uu = u*u*(3.0f-2.0f*u);
    float vv = v*v*(3.0f-2.0f*v);
    float ww = w*w*(3.0f-2.0f*w);
    float accum = 0;
    for(int i = 0; i < 2; i++) {
        for(int j = 0; j < 2; j++) {
            for(int k = 0; k < 2; k++) {
                vec3 weight_v(u-i, v-j, w-k);
                accum += (i*uu + (1.0f-i)*(1.0f-uu)) *
                         (j*vv + (1.0f-j)*(1.0f-vv)) *
                         (k*ww + (1.0f-k)*(1.0f-ww)) * dot(c[i][j][k], weight_v);
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

    vec3 c[2][2][2];
    for(int di = 0; di < 2; di++) {
        for(int dj = 0; dj < 2; dj++) {
            for(int dk = 0; dk < 2; dk++) {
                c[di][dj][dk] = ranvec[perm_x[(i+di) & 255] ^ perm_y[(j+dj) & 255] ^ perm_z[(k+dk) & 255]];
            }
        }
    }
    return perlin_interp(c, u, v, w);
}

static vec3* perlin_generate() {
    vec3* p = new vec3[256];
    for(int i = 0; i < 256; i++) {
        p[i] = normalize(vec3(-1.0f + 2.0f*drand48(), -1.0f + 2.0f*drand48(), -1.0f + 2.0f*drand48()));
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
    ranvec = perlin_generate();
    perm_x = perlin_generate_perm();
    perm_y = perlin_generate_perm();
    perm_z = perlin_generate_perm();
}

vec3* perlin::ranvec;
int* perlin::perm_x;
int* perlin::perm_y;
int* perlin::perm_z;
