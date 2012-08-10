#include <Arduino.h>

#include "colors.h"
#include "util.h"
#include "spectrum.h"

Spectrum::Spectrum(uint32_t *samples, int n_samples) {
    colors = (uint32_t *) calloc(n_colors, 4);
    if (colors != NULL) {
        for (int i = 0; i < n_samples; i++) {
            colors[i] = samples[i];
        }
    }
    n_colors = n_samples;
}

uint32_t Spectrum::at(float index) {
    if (index >= 1.) {
        return colors[n_colors - 1];
    }
    if (index <= 0.) {
        return colors[0];
    }
    index *= (n_colors - 1);
    int il = floor(index);
    int ih = ceil(index);
    if (ih > n_colors) {
        ih = n_colors - 1;
    }
    if (il < 0) {
        il = 0;
    }
    float r = index - il;
    return interpolate(colors[il], colors[ih], r);
}

Spectrum::~Spectrum(void) {
    if (colors != NULL) {
        free(colors);
    }
}
