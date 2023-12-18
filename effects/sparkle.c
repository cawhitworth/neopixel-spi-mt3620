#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>

#include "../pixels.h"
#include "effects.h"

#define NSPARKLES 50

typedef struct {
    int position;
    float phase;
    float trim;
    float speed;
} sparkle_t;

typedef struct
{
    char r;
    char g;
    char b;
} pix_t;

static sparkle_t sparkles[NSPARKLES];

static pix_t* display;
static size_t display_size;


static sparkle_t new_sparkle(size_t npix)
{
    sparkle_t s = {
        .position = rand() % npix,
        .phase = 0.f,
        .trim = 0.5f + (0.5f * frand()),
        .speed = (0.5f + frand()) / 100.f
    };
    return s;
}

static void draw_sparkle(const sparkle_t* s)
{
    float brightness = sinf(s->phase);
    brightness = (brightness < s->trim) ? 0.f : (brightness - s->trim) * (1.f / (1.f-s->trim));
    int r = display[s->position].r;
    int g = display[s->position].g;
    int b = display[s->position].b;
    r += (int)(brightness * 100.f);
    g += (int)(brightness * 100.f);
    //b += (int)(brightness * 255.f);
    if (r > 255) r = 255;
    if (g > 255) g = 255;
    if (b > 255) b = 255;
    display[s->position].r = r;
    display[s->position].g = g;
    display[s->position].b = b;
}

static void initialize(size_t npix)
{
    static bool Initialized = false;
    if (!Initialized)
    {
        display_size = npix * sizeof(pix_t);
        display = malloc(display_size);
        memset(display, 0, display_size);

        for(size_t s = 0; s < NSPARKLES; s++)
        {
            // TODO occupancy
            sparkles[s] = new_sparkle(npix);
        }
        Initialized = true;
    }
}

void sparkle(char* pixel_buffer, size_t pixel_buffer_len, size_t npix)
{
    initialize(npix);

    for(int p = 0; p < npix; p++) {
        display[p].r = 0;
        display[p].g = 0;
        display[p].b = 100;
    }

    for(size_t s = 0; s < NSPARKLES; s++)
    {
        draw_sparkle(&sparkles[s]);
        
        sparkles[s].phase += sparkles[s].speed;
        if (sparkles[s].phase > 3.141f) {
            sparkles[s] = new_sparkle(npix);
        }
    }

    for(size_t p = 0; p < npix; p++) {
        plot((unsigned char*)&display[p], p, pixel_buffer, pixel_buffer_len);
    }


}