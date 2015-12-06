#ifndef spectrum_h
#define spectrum_h

#include <stdint.h>

#define termColor 0x01000000

typedef struct {
    uint32_t *gamut;
    uint8_t size;
} Spectrum;

Spectrum createSpectrum(uint32_t *colors, int size);

Spectrum ensureUnique(Spectrum s);

uint32_t nextColor(Spectrum s, uint32_t c, bool wrap);

uint32_t prevColor(Spectrum s, uint32_t c, bool wrap);

void destroySpectrum(Spectrum s);

#endif
