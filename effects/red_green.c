#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#include "../pixels.h"
#include "effects.h"

#define NPHASES 6
static float phases[NPHASES];
static float speed[NPHASES];
static float WaveSize = 0.98f;

static void initialize(void)
{
    static bool Initialized = false;
    if (!Initialized)
    {
        for(unsigned int phase = 0; phase < NPHASES; phase++) {
            phases[phase] = 2.f * 3.141f * (1.f + frand() * 5.f);
            speed[phase] = 0.002f * (1.f + frand() * 5.f);
            if (phase % 2 == 0) speed[phase] = -speed[phase];
        }

        Initialized = true;
    }
}

void red_green(char* pixel_buffer, size_t pixel_buffer_len, size_t npix)
{
    initialize();

    for (unsigned int pix = 0; pix < npix; pix++) {
        float fpix = (float) pix / (float) npix;
        float fpix_r = fpix * 2.f * 3.141f;

        float cr = 0.5f;
        float cg = 0.f;
        float cb = 0.f;

        for(unsigned int phase = 0; phase < NPHASES; phase ++) {
            float h = sinf(fpix_r + phases[phase]);
            h = (h < WaveSize) ? 0.f : (h - WaveSize) * (1.f / (1.f-WaveSize));

            cr -= h/2;
            cg += h;
        }

        if (cr < 0.f) cr = 0.f;
        if (cg < 0.f) cg = 0.f;
        if (cb < 0.f) cb = 0.f;
        if (cr > 1.f) cr = 1.f;
        if (cg > 1.f) cg = 1.f;
        if (cb > 1.f) cb = 1.f;
        cr *= 255.f; cg *= 255.f; cb *= 255.f;

        //r *= 0.1; g *= 0.1; b *= 0.1;
        unsigned char p[] = { (unsigned char)cr, (unsigned char)cg, (unsigned char)cb };
        plot(p, pix, pixel_buffer, pixel_buffer_len);
    }

    for (unsigned int phase = 0; phase < NPHASES; phase ++) {
        phases[phase] += speed[phase];
        if (phases[phase] > 2 * 3.141f) phases[phase] -= 2 * 3.141f;
        if (phases[phase] < 0.f) phases[phase] += 2 * 3.141f;
    }
}