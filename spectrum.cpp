#include <Arduino.h>

#include "colors.h"
#include "util.h"
#include "spectrum.h"

uint32_t colorAt(uint32_t *colors, int nColors, float index) {
    if (index >= 1.) {
        return colors[nColors - 1];
    }
    if (index <= 0.) {
        return colors[0];
    }
    index *= (nColors - 1);
    int il = floor(index);
    int ih = ceil(index);
    if (ih >= nColors) {
        ih = nColors - 1;
    }
    if (il < 0) {
        il = 0;
    }
    float r = index - il;
    return interpolate(colors[il], colors[ih], r);
}

Spectrum createSpectrum(uint32_t *colors, int size) {
    int nColors = 0;
    while (colors[nColors] != termColor) {
        nColors++;
    }
    uint32_t *gamut = (uint32_t *) calloc(size, 4);
    for (int i = 0; i < size; i++) {
        gamut[i] = colorAt(colors, nColors, i / (float) (size - 1));
    }
    Spectrum s = {gamut: gamut, size: size};
    return s;
}

uint32_t prevColor(Spectrum s, uint32_t c, bool wrap) {
    uint32_t *gamut = s.gamut;
    int size = s.size;
    if (c == gamut[0]) {
        return wrap ? gamut[size - 1] : gamut[0];
    }
    for (int i = size - 1; i > 0; i--) {
        if (c == gamut[i]) {
            return gamut[i - 1];
        }
    }
    return gamut[0];
}

uint32_t nextColor(Spectrum s, uint32_t c, bool wrap) {
    uint32_t *gamut = s.gamut;
    int size = s.size;
    if (c == gamut[size - 1]) {
        return wrap ? gamut[0] : gamut[size - 1];
    }
    for (int i = 0; i < size - 1; i++) {
        if (c == gamut[i]) {
            return gamut[i + 1];
        }
    }
    return gamut[size - 1];
}

void destroySpectrum(Spectrum s) {
    if (s.gamut != NULL) {
        free(s.gamut);
    }
}
