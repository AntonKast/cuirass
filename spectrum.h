#ifndef spectrum_h
#define spectrum_h

#include <stdint.h>

class Spectrum {

    public:

    Spectrum(uint32_t *samples, int n_samples);
    ~Spectrum();
    uint32_t at(float index);

    uint32_t *colors;
    int n_colors;
};

#endif
