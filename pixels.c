#include "pixels.h"
#include <sys/types.h>

int plot(unsigned char pixel[3], unsigned int pix, unsigned char* p_buf, size_t p_buf_len) {
    static const unsigned char BIT0 = 0b11000000;
    static const unsigned char BIT1 = 0b11110000;

    unsigned int base = pix * BPP * 8;

    // Nanopixels in GRB order, of course
    static const size_t index[] = { 1, 0, 2 };

    for(size_t idx = 0; idx<3; idx++) {
        if (base >= p_buf_len) {
            return -1;
        }

        unsigned char channel = pixel[index[idx]];
        for(ssize_t bit = 7; bit >= 0; bit --) {
            p_buf[base++] = ((channel >> bit) & 0x01) ? BIT1 : BIT0;
        }
    }

    return 0;
}
