#include <stdlib.h>
#include <math.h>

#include "../pixels.h"

void rainbow(char* pixel_buffer, size_t pixel_buffer_len, size_t npix)
{
    static float phase_r = 0.0f;
    static float phase_g = 0.3f * 2.f * 3.141f;
    static float phase_b = 0.6f * 2.f * 3.141f;

    for (unsigned int pix = 0; pix < npix; pix++) {
        float fpix = (float) pix / (float) npix;
        float fpix_r = fpix * 2.f * 3.141f;
        float r = 127.f + (sinf(fpix_r + phase_r) * 255.f);
        float g = 127.f + (sinf(fpix_r + phase_g) * 255.f);
        float b = 127.f + (sinf(fpix_r + phase_b) * 255.f);
        r = (r < 0.f) ? 0.f : (r > 255.f) ? 255.f : r;
        g = (g < 0.f) ? 0.f : (g > 255.f) ? 255.f : g;
        b = (b < 0.f) ? 0.f : (b > 255.f) ? 255.f : b;

        //r *= 0.1; g *= 0.1; b *= 0.1;
        unsigned char p[] = { (unsigned char)r, (unsigned char)g, (unsigned char)b };
        plot(p, pix, pixel_buffer, pixel_buffer_len);
    }

    phase_r += 0.01f;
    phase_g += 0.01f;
    phase_b += 0.01f;
    if (phase_r > 2 * 3.141f) phase_r -= 2 * 3.141f;
    if (phase_g > 2 * 3.141f) phase_g -= 2 * 3.141f;
    if (phase_b > 2 * 3.141f) phase_b -= 2 * 3.141f;
}
