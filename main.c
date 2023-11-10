// This minimal Azure Sphere app prints "High Level Application" to the debug
// console and exits with status 0.
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <errno.h>

#include <applibs/log.h>
#include <applibs/spi.h>

#include "hw/neopixel.h"
#include "effects/effects.h"
#include "pixels.h"

#define NPIX 60
#define FREQ 6400000
#define TRESET (0.00008)

#define RESETBUFLEN ((size_t)(FREQ * TRESET / 8))
#define PIXBUFLEN 8 * NPIX * BPP

static unsigned char ResetBuffer[RESETBUFLEN];
static unsigned char PixelBuffer[PIXBUFLEN];


int main(void)
{
    /* H4P11 MOSI */
    SPIMaster_Config config;
    int ret = SPIMaster_InitConfig(&config);
    if (ret != 0) {
        Log_Debug("SPIMaster_InitConfig: %s (%d)\n", strerror(errno), errno);
        return -1;
    }

    int spiFd = SPIMaster_Open(NPX_SPI, NPX_SPI_CS, &config);
    ret = SPIMaster_SetBusSpeed(spiFd, FREQ);
    ret = SPIMaster_SetMode(spiFd, SPI_Mode_0); //Not needed

    // Reset

    memset(ResetBuffer, 0, RESETBUFLEN);

    SPIMaster_WriteThenRead(spiFd, ResetBuffer, RESETBUFLEN, ResetBuffer, RESETBUFLEN);

    // Update 50Hz
    struct timespec sleep = {
        .tv_sec = 0,
        .tv_nsec = 20000000,
    };


    while(1==1) {
        blue_wave(PixelBuffer, PIXBUFLEN, NPIX);

        SPIMaster_WriteThenRead(spiFd, PixelBuffer, PIXBUFLEN, PixelBuffer, PIXBUFLEN);
        nanosleep(&sleep, 0);

    }

    return 0;

}