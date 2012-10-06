#ifndef colors_h
#define colors_h

#include <Arduino.h>
#include <stdint.h>

// full brightness
//#define white 0x00FFFFFF
//#define red   0x00FF0000
//#define green 0x0000FF00
//#define blue  0x000000FF
//#define black 0x00000000
//#define grayshift 0

// halve colors--full power is too bright
//#define white 0x00808080
//#define red   0x00800000
//#define green 0x00008000
//#define blue  0x00000080
//#define black 0x00000000
//#define grayshift 1

// quarter colors--even dimmer
#define white 0x00404040
#define red   0x00400000
#define green 0x00004000
#define blue  0x00000040
#define black 0x00000000
#define grayshift 2

uint32_t color(byte r, byte g, byte b);
uint32_t gray(byte g);
uint32_t graylevel(int l);
uint32_t interpolate(uint32_t c1, uint32_t c2, float w);

#endif
