#include "colors.h"

uint32_t white  = gray(255);
uint32_t black  = gray(0);
uint32_t grays[] = {
    white, 
    gray(128), gray(64), gray(32), gray(16), gray(8), gray(4), gray(2), gray(1),
    black
};

uint32_t red   = 0x00FF0000;
uint32_t green = 0x0000FF00;
uint32_t blue  = 0x000000FF;

uint32_t yellow  = interpolate(red,   green, .5);
uint32_t magenta = interpolate(blue,  red,   .5);
uint32_t cyan    = interpolate(green, blue,  .5);

uint32_t orange = interpolate(green, red, 5 / 6.);

uint32_t termColor = 0x01000000;

bool isTermColor(uint32_t c) {
    return (c >> 24) > 0;
}

uint32_t color(byte r, byte g, byte b) {
//    return ((((uint32_t) r << 8) | g) << 8) | b;
    uint32_t c = r;
    c <<= 8;
    c |= g;
    c <<= 8;
    c |= b;
    return c;
}

uint32_t gray(byte g) {
    return color(g, g, g);
}

uint32_t interpolate(uint32_t c1, uint32_t c2, float w) {
//    int r1 = c1 >> 16 & 0xFF;
//    int g1 = c1 >>  8 & 0xFF;
//    int b1 = c1 >>  0 & 0xFF;
//    int r2 = c2 >> 16 & 0xFF;
//    int g2 = c2 >>  8 & 0xFF;
//    int b2 = c2 >>  0 & 0xFF;

    int r1 = (c1 & 0x00FF0000) >> 16;
    int g1 = (c1 & 0x0000FF00) >>  8;
    int b1 = (c1 & 0x000000FF) >>  0;
    int r2 = (c2 & 0x00FF0000) >> 16;
    int g2 = (c2 & 0x0000FF00) >>  8;
    int b2 = (c2 & 0x000000FF) >>  0;

    int r = (int) (r1 * (1 - w) + r2 * w);
    int g = (int) (g1 * (1 - w) + g2 * w);
    int b = (int) (b1 * (1 - w) + b2 * w);

    return color(r, g, b);
}
