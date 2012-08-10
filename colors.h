#ifndef colors_h
#define colors_h

#include <Arduino.h>
#include <stdint.h>

extern uint32_t white;
extern uint32_t grays[];
extern uint32_t black;

extern uint32_t red;
extern uint32_t green;
extern uint32_t blue;
extern uint32_t yellow;
extern uint32_t magenta;
extern uint32_t cyan;

extern uint32_t orange;

uint32_t color(byte r, byte g, byte b);
uint32_t gray(byte g);
uint32_t interpolate(uint32_t c1, uint32_t c2, float w);

#endif
