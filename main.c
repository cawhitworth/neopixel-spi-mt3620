// This minimal Azure Sphere app prints "High Level Application" to the debug
// console and exits with status 0.
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <applibs/log.h>
#include <applibs/spi.h>
#include "hw/neopixel.h"

#define BPP 3
#define NPIX 60
#define FREQ 6400000
#define TRESET 80e-6f

#define RESETBUFLEN ((size_t)(FREQ * TRESET / 8))
#define PIXBUFLEN 8 * NPIX * BPP

static unsigned char reset_buffer[RESETBUFLEN];
static unsigned char pixel_buffer[PIXBUFLEN];
static unsigned char bit0 = 0b11000000;
static unsigned char bit1 = 0b11110000;

int plot(unsigned char pixel[3], unsigned int pix, unsigned char* p_buf, size_t p_buf_len) {
    int base = pix * BPP * 8;

    // Nanopixels in GRB order, of course
    static const size_t index[] = { 1, 0, 2 };

    for(size_t idx = 0; idx<3; idx++) {
        if (base >= p_buf_len) {
            return -1;
        }

        unsigned char channel = pixel[index[idx]];
        for(ssize_t bit = 7; bit >= 0; bit --) {
            p_buf[base++] = ((channel >> bit) & 0x01) ? bit1 : bit0;
        }
    }

    return 0;
}

int main(void)
{
    /* H4P11 MOSI */
    SPIMaster_Config config;
    int ret = SPIMaster_InitConfig(&config);
    int spiFd = SPIMaster_Open(NPX_SPI, NPX_SPI_CS, &config);
    ret = SPIMaster_SetBusSpeed(spiFd, FREQ);
    ret = SPIMaster_SetMode(spiFd, SPI_Mode_0); //Not needed

    // Reset

    memset(reset_buffer, 0, RESETBUFLEN);
    
    SPIMaster_WriteThenRead(spiFd, reset_buffer, RESETBUFLEN, reset_buffer, RESETBUFLEN);

    // Update 50Hz
    struct timespec sleep = {
        .tv_sec = 0,
        .tv_nsec = 20000000,
    };

    float phase_r = 0.0f;
    float phase_g = 0.3 * 2 * 3.141f;
    float phase_b = 0.6 * 2 * 3.141f;

    while(1==1) {
        
        for (int pix = 0; pix < NPIX; pix++) {
            float fpix = (float) pix / (float) NPIX;
            float fpix_r = fpix * 2 * 3.141;
            float r = 127 + (sinf(fpix_r + phase_r) * 255);
            float g = 127 + (sinf(fpix_r + phase_g) * 255);
            float b = 127 + (sinf(fpix_r + phase_b) * 255);
            r = (r < 0) ? 0 : (r > 255) ? 255 : r;
            g = (g < 0) ? 0 : (g > 255) ? 255 : g;
            b = (b < 0) ? 0 : (b > 255) ? 255 : b;

            //r *= 0.1; g *= 0.1; b *= 0.1;
            unsigned char p[] = { r, g, b };
            plot(p, pix, pixel_buffer, PIXBUFLEN);
        }

        SPIMaster_WriteThenRead(spiFd, pixel_buffer, PIXBUFLEN, pixel_buffer, PIXBUFLEN);
        nanosleep(&sleep, 0);

        phase_r += 0.01;
        phase_g += 0.01;
        phase_b += 0.01;
        if (phase_r > 2 * 3.141f) phase_r -= 2 * 3.141f;
        if (phase_g > 2 * 3.141f) phase_g -= 2 * 3.141f;
        if (phase_b > 2 * 3.141f) phase_b -= 2 * 3.141f;
    }

    return 0;

}