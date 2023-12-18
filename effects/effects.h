#pragma once

#include <stdlib.h>

static inline float frand(void)
{
    return ((float)rand() / (float)RAND_MAX);
}

extern void blue_wave(char* pixel_buffer, size_t pixel_buffer_len, size_t npix);
extern void rainbow(char* pixel_buffer, size_t pixel_buffer_len, size_t npix);
extern void sparkle(char* pixel_buffer, size_t pixel_buffer_len, size_t npix);
extern void red_green(char* pixel_buffer, size_t pixel_buffer_len, size_t npix);