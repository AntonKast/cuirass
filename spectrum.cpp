#include <Arduino.h>

#include "colors.h"
#include "util.h"
#include "spectrum.h"

Spectrum::Spectrum(uint32_t *colors, int nGamut) {
    nColors_ = 0;
    while (! isTermColor(colors[nColors_])) {
        nColors_++;
    }
    colors_ = (uint32_t *) calloc(nColors_, 4);
    if (colors != NULL) {
        for (int i = 0; i < nColors_; i++) {
            colors_[i] = colors[i];
        }
    }
    nGamut_ = nGamut;
    gamut_ = (uint32_t *) calloc(nGamut_, 4);
    for (int i = 0; i < nGamut_; i++) {
        gamut_[i] = at(i / (float) (nGamut_ - 1));
    }
}

uint32_t Spectrum::at(float index) {
    if (index >= 1.) {
        return colors_[nColors_ - 1];
    }
    if (index <= 0.) {
        return colors_[0];
    }
    index *= (nColors_ - 1);
    int il = floor(index);
    int ih = ceil(index);
    if (ih > nColors_) {
        ih = nColors_ - 1;
    }
    if (il < 0) {
        il = 0;
    }
    float r = index - il;
    return interpolate(colors_[il], colors_[ih], r);
}

uint32_t *Spectrum::gamut() {
    return gamut_;
}

int Spectrum::nGamut() {
    return nGamut_;
}

uint32_t Spectrum::prev(uint32_t c, bool wrap = false) {
    if (c == gamut_[0]) {
        return wrap ? gamut_[nGamut_ - 1] : gamut_[0];
    }
    for (int i = nGamut_; i > 0; i--) {
        if (c == gamut_[i]) {
            return gamut_[i - 1];
        }
    }
    return gamut_[0];
}

uint32_t Spectrum::next(uint32_t c, bool wrap = false) {
    if (c == gamut_[nGamut_ - 1]) {
        return wrap ? gamut_[0] : gamut_[nGamut_ - 1];
    }
    for (int i = nGamut_; i > 0; i--) {
        if (c == gamut_[i]) {
            return gamut_[i - 1];
        }
    }
    return gamut_[0];
}

Spectrum::~Spectrum(void) {
    if (colors_ != NULL) {
        free(colors_);
    }
    if (gamut_ != NULL) {
        free(gamut_);
    }
}
