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

static unsigned char pixel_buffer[8 * NPIX * BPP];
static unsigned char bit0 = 0b11000000;
static unsigned char bit1 = 0b11110000;

int plot(unsigned char pixel[3], unsigned int pix, unsigned char* p_buf, size_t p_buf_len) {
    int base = pix * BPP * 8;
    size_t index[] = { 1, 0, 2 };

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
    size_t resetBufLen = FREQ * TRESET / 8;

    unsigned char* resetBuf = malloc(resetBufLen);
    memset(resetBuf, 0, resetBufLen);
    
    SPIMaster_WriteThenRead(spiFd, resetBuf, resetBufLen, resetBuf, resetBufLen);

    int base = 0;
    for(int i = 0; i < NPIX; i++) {
        for(int g = 0; g < 8; g++) {
            pixel_buffer[base++] = bit0;
        }
        for(int r = 0; r < 8; r++) {
            pixel_buffer[base++] = bit1;
        }
        for(int b = 0; b < 8; b++) {
            pixel_buffer[base++] = bit0;
        }
    }

    size_t len = sizeof(pixel_buffer);
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
            unsigned char p[] = { r, g, b };
            plot(p, pix, pixel_buffer, len);
        }
        SPIMaster_WriteThenRead(spiFd, pixel_buffer, len, pixel_buffer, len);
        nanosleep(&sleep, 0);
        phase_r += 0.05;
        phase_g += 0.05;
        phase_b += 0.05;
        if (phase_r > 2 * 3.141f) phase_r -= 2 * 3.141f;
        if (phase_g > 2 * 3.141f) phase_g -= 2 * 3.141f;
        if (phase_b > 2 * 3.141f) phase_b -= 2 * 3.141f;
    }

    Log_Debug("Success: %d", ret);
    return 0;

}