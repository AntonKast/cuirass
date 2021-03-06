#include "colors.h"

uint32_t white = 0x00FFFFFF;
uint32_t red   = 0x00FF0000;
uint32_t green = 0x0000FF00;
uint32_t blue  = 0x000000FF;
uint32_t black = 0x00000000;

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
    if (l <= 8) {
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
