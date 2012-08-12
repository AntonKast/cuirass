#ifndef colors_h
#define colors_h

#include <Arduino.h>
#include <stdint.h>

#define white 0x00FFFFFF
#define red   0x00FF0000
#define green 0x0000FF00
#define blue  0x000000FF
#define black 0x00000000

#define termColor 0x01000000

uint32_t color(byte r, byte g, byte b);
uint32_t gray(byte g);
uint32_t graylevel(int l);
uint32_t interpolate(uint32_t c1, uint32_t c2, float w);

bool isTermColor(uint32_t c);

#endif
