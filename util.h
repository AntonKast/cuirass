#ifndef util_h
#define util_h

#include <stdint.h>
#include <Arduino.h>
#include "spectrum.h"

#define MID_START 0
#define MID_END 131

#define LEFT_START 132
#define LEFT_END 170

#define RIGHT_START 171
#define RIGHT_END 209

void left(uint32_t c);
void right(uint32_t c);
void top(uint32_t c);
void mid(uint32_t c);
int midLogicToIndex(int x, int y);
int leftLogicToIndex(int x, int y);
int rightLogicToIndex(int x, int y);
void leftSetPixels(int pairs[][2], int n_pixels, uint32_t c);
void rightSetPixels(int pairs[][2], int n_pixels, uint32_t c);
extern int ringPairs[16][2];
void leftRow(int row, uint32_t c);
void rightRow(int row, uint32_t c);
void leftRing(uint32_t c);
void rightRing(uint32_t c);
void shiftDown();
void shiftLeft();
void dim();
void dimLess();
void dimFloat(float f);
void topDim(float f);
void fadeTowards(uint32_t t, float f);
void midDim();
void midDimLess();
void midDimFloat(float f);
uint32_t pixelDim(uint32_t c);
uint32_t pixelDimLess(uint32_t c);
uint32_t pixelDimFloat(uint32_t c, float f);
void horizontal(int row, uint32_t c);
void vertical(int col, uint32_t c);
void square(int size, uint32_t c);
void ranxels(Spectrum &s);
uint32_t ranxel(Spectrum &s);
uint32_t ranxel();
void solid(uint32_t c);
void gradient(uint32_t cl, uint32_t ch);
void gradient(Spectrum &s);
void midSolid(uint32_t c);
uint32_t wheel(byte pos);
uint32_t getPixel(int i);
void setPixel(int i, uint32_t c);
void setPixels(int *indices, int n_indices, uint32_t c);
void setPixel(int x, int y, uint32_t c);
float exponential(float lambda);
uint32_t decay(uint32_t c);
void setTimeout(unsigned long m);
bool isTimeout();
void clearTimeout();
void rotate(Spectrum &s, bool reverse);
void rotateMid(Spectrum &s, bool reverse);
void rotateTop(Spectrum &s, bool reverse);
void rotate(Spectrum &s, int i, bool reverse);

#endif
