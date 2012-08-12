#include "colors.h"

bool isTermColor(uint32_t c) {
    return (c >> 24) > 0;
}

uint32_t color(byte r, byte g, byte b) {
    return ((((uint32_t) r << 8) | g) << 8) | b;
}

uint32_t gray(byte g) {
    return color(g, g, g);
}

uint32_t graylevel(int l) {
    if (l <= 0) {
        return white;
    }
    if (l <= 24) {
        return 0x01010100 >> l;
    }
    return black;
}

uint32_t interpolate(uint32_t c1, uint32_t c2, float w) {
    int r1 = c1 >> 16 & 0xFF;
    int g1 = c1 >>  8 & 0xFF;
    int b1 = c1 >>  0 & 0xFF;
    int r2 = c2 >> 16 & 0xFF;
    int g2 = c2 >>  8 & 0xFF;
    int b2 = c2 >>  0 & 0xFF;

    int r = (int) (r1 * (1 - w) + r2 * w);
    int g = (int) (g1 * (1 - w) + g2 * w);
    int b = (int) (b1 * (1 - w) + b2 * w);

    return color(r, g, b);
}
