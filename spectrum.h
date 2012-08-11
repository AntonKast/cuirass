#ifndef spectrum_h
#define spectrum_h

#include <stdint.h>

class Spectrum {

    public:

    Spectrum(uint32_t *colors, int nGamut);
    ~Spectrum();

    uint32_t at(float index);

    uint32_t *gamut();
    int nGamut();

    uint32_t prev(uint32_t c, bool wrap);
    uint32_t next(uint32_t c, bool wrap);

    private:

    uint32_t *colors_;
    int nColors_;

    uint32_t *gamut_;
    int nGamut_;
};

#endif
