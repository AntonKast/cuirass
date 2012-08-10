#include <SPI.h>
#include <Adafruit_WS2801.h>

#include "util.h"
#include "colors.h"
#include "spectrum.h"

// strip initialization

int dataPin  = 2;    // yellow wire
int clockPin = 3;    // green wire

int numPixels = 210;

Adafruit_WS2801 strip = Adafruit_WS2801(numPixels, dataPin, clockPin);

//
// gamut-related stuff: move all but globals into Spectrum
//

#define n_gamut 100

void rotate(uint32_t (*next)(uint32_t)) {
    for (int i = 0; i < strip.numPixels(); i++) {
        uint32_t c = strip.getPixelColor(i);
        c = (*next)(c);
        strip.setPixelColor(i, c);
    }
}

// for effectPlanckFlash:
// #define n_gamut 100
uint32_t planck_colors[] = {
    black,
    interpolate(black, red, .0125),
    interpolate(black, red, .025),
    interpolate(black, red, .05),
    interpolate(black, red, .1),
    interpolate(black, red, .2),
    interpolate(black, red, .5),
    orange,
    cyan
};

// for effectMatrix
// #define n_gamut 10
uint32_t green_shades[] = {
    black,
    interpolate(black, green, .02),
    interpolate(black, green, .1),
    green
};

Spectrum green_spectrum = Spectrum(green_shades, 4);

Spectrum planck_spectrum = Spectrum(planck_colors, 9);

uint32_t planck_gamut[n_gamut + 1];

uint32_t green_gamut[n_gamut + 1];

uint32_t planck_prev(uint32_t c) {
    if (c == black) {
        return black;
    }
    for (int i = n_gamut; i > 0; i--) {
        if (c == planck_gamut[i]) {
            return planck_gamut[i - 1];
        }
    }
    return black;
}

uint32_t green_prev(uint32_t c) {
    if (c == black) {
        return black;
    }
    for (int i = n_gamut; i > 0; i--) {
        if (c == green_gamut[i]) {
            return green_gamut[i - 1];
        }
    }
    return black;
}

//
// END gamut-related stuff
//

void setup() {    
    strip.begin();
    strip.show();
    for (int i = 0; i <= n_gamut; i++) {
        planck_gamut[i] = planck_spectrum.at(i / (float) n_gamut);
    }
    for (int i = 0; i <= n_gamut; i++) {
        green_gamut[i] = green_spectrum.at(i / (float) n_gamut);
    }
}

void loop() {
    effectCrazyColors();
}

byte fearText[] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    1, 1, 1, 0, 1, 1, 1, 0, 0, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0,
    1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0,
    1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0,
    1, 1, 1, 0, 1, 1, 0, 0, 1, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0,
    1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0,
    1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0,
    1, 0, 0, 0, 1, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

void effectCrawlText() {
    horizontal(0, grays[2]);
    horizontal(1, grays[2]);
    int row = 0;
    for (int i = 0; i < 20; i++) {
        shiftLeft();
        for (int y = 0; y < 12; y++) {
            if (fearText[20 * y + i] > 0) {
                setPixel(11, 11 - y, black);
            }
            else {
                setPixel(11, 11 - y, grays[2]);
            }
        }
        top(grays[2]);
        leftRow(row, black);
        leftRow((row + 3) % 7, black);
        rightRow(row, black);
        rightRow((row + 3) % 7, black);
        row = (row + 1) % 7;
        strip.show();
        delay(100);
    }
}

void effectFire() {
    for (int row = 0; row < 6; row++) {
        solid(black);
        leftRow(row, red);
        rightRow(row, red);
        horizontal(row, red);
        horizontal(row + 6, red);
        strip.show();
    }
}

void effectMatrix() {
    uint32_t c1 = green_gamut[n_gamut - 1];
    uint32_t c2 = green_gamut[n_gamut - 2];
    for (int x = 0; x < 12; x++) {
        if (random(20) == 0) {
            setPixel(x, 11, c1);
        }
    }
    for (int x = 0; x < 12; x++) {
        for (int y = 11; y > 0; y--) {
            int i = midLogicToIndex(x, y);
            uint32_t c = getPixel(i);
            if (c == c2) {
                setPixel(x, y - 1, c1);
            }
        }
    }
    strip.show();
    rotate(green_prev);
    delay(50);
}

void effectMatrixFire() {
    uint32_t c1 = planck_gamut[n_gamut];
    uint32_t c2 = planck_gamut[n_gamut - 1];
    for (int x = 0; x < 12; x++) {
        if (random(20) == 0) {
            setPixel(x, 11, c1);
        }
    }
    for (int x = 0; x < 12; x++) {
        for (int y = 11; y > 0; y--) {
            int i = midLogicToIndex(x, y);
            uint32_t c = getPixel(i);
            if (c == c2) {
                setPixel(x, y - 1, c1);
            }
        }
    }
    strip.show();
    rotate(planck_prev);
    delay(100);
}

void effectRedWhiteBlue() {
//    left(blue);
//    right(blue);

    int il = random(LEFT_END - LEFT_START + 1) + LEFT_START;
    int ir = random(RIGHT_END - RIGHT_START + 1) + RIGHT_START;
    strip.setPixelColor(il, white);
    strip.setPixelColor(ir, white);

    for (int i = LEFT_START; i <= LEFT_END; i++) {
        uint32_t c = getPixel(i);
        if (c != blue) {
            c = interpolate(c, blue, .1);
            setPixel(i, c);
        }
    }
    for (int i = RIGHT_START; i <= RIGHT_END; i++) {
        uint32_t c = getPixel(i);
        if (c != blue) {
            c = interpolate(c, blue, .1);
            setPixel(i, c);
        }
    }
    horizontal(0, red);
    horizontal(1, grays[2]);
    horizontal(2, red);
    horizontal(3, grays[2]);
    horizontal(4, red);
    horizontal(5, grays[2]);
    horizontal(6, red);
    horizontal(7, grays[2]);
    horizontal(8, red);
    horizontal(9, grays[2]);
    horizontal(10, red);
    horizontal(11, grays[2]);

    strip.show();
}

// for effectRanxels--integrate into Spectrum
uint32_t orange_shades[] = {
    black,
    interpolate(black, orange, .02),
    interpolate(black, orange, .1),
    orange
};
Spectrum orange_spectrum = Spectrum(orange_shades, 4);

void effectRanxels() {
    for (int i = 0; i < 8; i++) {
        ranxels(orange_spectrum);
        top(orange_spectrum.at(i / 8.));
        strip.show();
    }
    for (int i = 7; i >= 0; i--) {
        ranxels(orange_spectrum);
        top(orange_spectrum.at(i / 8.));
        strip.show();
    }
}

void effectSwipeFadingPlanckRanxels() {
    uint32_t first = planck_gamut[n_gamut];
    uint32_t last = planck_gamut[0];
    for (int x = 0; x < 12; x++) {
        vertical(x, first);
        if (x > 0) {
            vertical(x - 1, black);
            for (int y = 0; y < 12; y++) {
                if (random(2) == 0) {
                    setPixel(x - 1, y, first);
                }
            }
        }
        rotate(planck_prev);
        strip.show();
    }
    vertical(11, last);
    for (int y = 0; y < 12; y++) {
        if (random(2) == 0) {
            setPixel(11, y, first);
        }
    }
    for (int n = 0; n < 200; n++) {
        rotate(planck_prev);
        strip.show();
    }
}

void effectFadingPlanckPixels() {
    solid(black);
    for (int n = 0; n < 100; n++) {
        setPixel(random(210), planck_gamut[n_gamut - 1]);
    }
    strip.show();
    for (int n = 0; n < 100; n++) {
        rotate(planck_prev);
        strip.show();
    }
    delay(1000);
}

void effectMouth() {
    for (int d=1; d<=6; d+=1) {
        solid(black);
        square(d, grays[2]);
        strip.show();
    }
    for (int d=6; d>=1; d-=1) {
        solid(black);
        square(d, grays[2]);
        strip.show();
    }
}

void effectSeizure() {
    solid(black);
    for (int i = 0; i < numPixels; i += 2) {
        setPixel(i, white);
    }
    strip.show();
    solid(black);
    for (int i = 1; i < numPixels; i += 2) {
        setPixel(i, white);
    }
    strip.show();
}

void effectChecker() {
    solid(black);
    for (int i = 0; i < numPixels; i += 2) {
        setPixel(i, white);
    }
    strip.show();
    delay(200);
    solid(black);
    for (int i = 1; i < numPixels; i += 2) {
        setPixel(i, white);
    }
    strip.show();
    delay(200);
}

void effectFlare() {
    if (isTimeout()) {
        solid(color(255, 32, 0));
        uint16_t t = (uint16_t) exponential(100.);
        setTimeout(t);
    }
    dimFloat(.8);
    strip.show();
}

void effectFadingRanxels() {
    setPixel(random(210), blue);
    dimFloat(.99);
    strip.show();
}

void effectFlicker() {
    solid(white);
    strip.show();
    delay(random(100));
    solid(black);
    strip.show();
    delay(random(100));
}

void effectPlanckFlash() {
    for (int n = n_gamut; n >= 0; n--) {
        solid(planck_gamut[n]);
        strip.show();
    }
    delay(1000);
}

void effectFlash() {
    solid(white);
    strip.show();
    delay(300);
    uint32_t c = strip.getPixelColor(0);
    while (c > 0) {
        dimFloat(.9);
        strip.show();
        c = strip.getPixelColor(0);
    }
    solid(black);
    strip.show();
    delay(1000);
}

void effectRainbow() {
    for (int j = 0; j < 256; j++) {     // 3 cycles of all 256 colors in the wheel
        for (int i = 0; i < strip.numPixels(); i++) {
            strip.setPixelColor(i, wheel( (i + j) % 255));
        }
        strip.show();   // write all the pixels out
    }
}

void effectRainbowCycle() {
    for (int j=0; j < 256 * 5; j++) {     // 5 cycles of all 25 colors in the wheel
        for (int i=0; i < strip.numPixels(); i++) {
            // tricky math! we use each pixel as a fraction of the full 96-color wheel
            // (thats the i / strip.numPixels() part)
            // Then add in j which makes the colors go around per pixel
            // the % 96 is to make the wheel cycle around
            strip.setPixelColor(i, wheel( ((i * 256 / strip.numPixels()) + j) % 256) );
        }  
        strip.show();   // write all the pixels out
    }
}

void effectCrazyColors() {
    uint32_t co = strip.getPixelColor(0);
    uint32_t cn = ranxel();
    for (int i = 0; i < 15; i++) {
        solid(interpolate(co, cn, i / 15.));
        strip.show();
    }
}
