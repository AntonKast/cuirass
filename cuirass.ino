#include <SPI.h>
#include <Adafruit_WS2801.h>

#include "util.h"
#include "colors.h"
#include "text.h"
#include "spectrum.h"

// initialize strip

int dataPin  = 2;    // yellow wire
int clockPin = 3;    // green wire

int numPixels = 210;

Adafruit_WS2801 strip = Adafruit_WS2801(numPixels, dataPin, clockPin);

// initializer spectra

uint32_t planckColors[] = {
    black,
    interpolate(black, red, .1),
    interpolate(black, red, .2),
    interpolate(black, red, .5),
    orange,
    cyan,
    termColor
};

uint32_t greenShades[] = {
    black,
    interpolate(black, green, .02),
    interpolate(black, green, .1),
    green,
    termColor
};

uint32_t orangeShades[] = {
    black,
    interpolate(black, orange, .02),
    interpolate(black, orange, .1),
    orange,
    termColor
};

uint32_t rainbowColors[] = {
    red,
    green,
    blue,
    interpolate(blue, red, .99),
    termColor
};

Spectrum planckSpectrum = Spectrum(planckColors, 100);
Spectrum greenSpectrum = Spectrum(greenShades, 10);
Spectrum orangeSpectrum = Spectrum(orangeShades, 0);
Spectrum rainbowSpectrum = Spectrum(rainbowColors, 40);

// main loop

void setup() {    
    strip.begin();
    strip.show();
}

void loop() {
//    effectFireworks();
//    effectSignature();
    effectRainbowFrame();
//    effectCrawlText();
//    effectBlinkText();
//    effectFire();
//    effectMatrix();
//    effectMatrixFire();
//    effectRedWhiteBlue();
//    effectRanxels();
//    effectSwipeFadingPlanckRanxels();
//    effectFadingPlanckPixels();
//    effectMouth();
//    effectSeizure();
//    effectBlink();
//    effectChecker();
//    effectFlare();
//    effectFadingRanxels();
//    effectFlicker();
//    effectPlanckFlash();
//    effectFlash();
//    effectRainbow();
//    effectRainbowCycle();
//    effectCrazyColors();
}

// effects

void effectFireworks() {
    if (isTimeout()) {
        uint32_t *gamut = planckSpectrum.gamut();
        int nGamut = planckSpectrum.nGamut();
        top(gamut[nGamut - 2]);
        // start down-shifting sparks at mid
        uint16_t t = (uint16_t) exponential(3000.);
        setTimeout(t);
    }
    rotate(planckSpectrum, false);
    strip.show();
}

void effectSignature() {
    solid(black);
    uint32_t *gamut = rainbowSpectrum.gamut();
    int n = 0;
    for (int x = 0; x < 12; x++) {
        setPixel(x, 2, gamut[n++]);
    }
    for (int y = 3; y < 12; y++) {
        setPixel(11, y, gamut[n++]);
    }
    for (int x = 10; x >= 0; x--) {
        setPixel(x, 11, gamut[n++]);
    }
    for (int y = 10; y > 2; y--) {
        setPixel(0, y, gamut[n++]);
    }
    for (n = 0; n < 1000; n++) {

        for (int x = 0; x < 12; x++) {
            rotate(rainbowSpectrum, midLogicToIndex(x, 2), true);
        }
        for (int y = 3; y < 12; y++) {
            rotate(rainbowSpectrum, midLogicToIndex(11, y), true);
        }
        for (int x = 10; x >= 0; x--) {
            rotate(rainbowSpectrum, midLogicToIndex(x, 11), true);
        }
        for (int y = 10; y > 2; y--) {
            rotate(rainbowSpectrum, midLogicToIndex(0, y), true);
        }
        bool toggle = n % 2 == 0;
        bool ak = (n / 20) % 2 == 0;

        for (int x = 1; x < 11; x++) {
            for (int y = 3; y < 11; y++) {
                bool on = ak ?
                    (aText[12 * (11 - y) + x] != 0) : 
                    (kText[12 * (11 - y) + x] != 0);
                setPixel(x, y, on & toggle ? grays[2] : black);
            }
        }
        strip.show();
    }
}

void effectRainbowFrame() {
    solid(black);
    uint32_t *gamut = rainbowSpectrum.gamut();
    int n = 0;
    for (int x = 0; x < 12; x++) {
        setPixel(x, 2, gamut[n++]);
    }
    for (int y = 3; y < 12; y++) {
        setPixel(11, y, gamut[n++]);
    }
    for (int x = 10; x >= 0; x--) {
        setPixel(x, 11, gamut[n++]);
    }
    for (int y = 10; y > 2; y--) {
        setPixel(0, y, gamut[n++]);
    }
    n = 0;
    for (int i = 0; i < 16; i++) {
        int *pair = ringPairs[i];
        setPixel(leftLogicToIndex(pair[0], pair[1]), gamut[n++]);
        setPixel(rightLogicToIndex(pair[0], pair[1]), gamut[n++]);
    }
    for (int i = 0; i < 1000; i++) {

        for (int x = 0; x < 12; x++) {
            rotate(rainbowSpectrum, midLogicToIndex(x, 2), true);
        }
        for (int y = 3; y < 12; y++) {
            rotate(rainbowSpectrum, midLogicToIndex(11, y), true);
        }
        for (int x = 10; x >= 0; x--) {
            rotate(rainbowSpectrum, midLogicToIndex(x, 11), true);
        }
        for (int y = 10; y > 2; y--) {
            rotate(rainbowSpectrum, midLogicToIndex(0, y), true);
        }
        for (int i = 0; i < 16; i++) {
            int *pair = ringPairs[i];
            rotate(rainbowSpectrum, leftLogicToIndex(pair[0], pair[1]), true);
            rotate(rainbowSpectrum, rightLogicToIndex(pair[0], pair[1]), true);
        }
        strip.show();
    }
}

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

void effectBlinkText() {
    for (int n = 0; n < 30; n++) {
        bool toggle = n % 2 == 0;
        for (int x = 0; x < 12; x++) {
            for (int y = 2; y < 12; y++) {
                bool on = (aText[12 * (11 - y) + x] != 0);
                setPixel(x, y, on & toggle ? grays[2] : black);
            }
        }
        strip.show();
    }
    for (int n = 0; n < 30; n++) {
        bool toggle = n % 2 == 0;
        for (int x = 0; x < 12; x++) {
            for (int y = 2; y < 12; y++) {
                bool on = (kText[12 * (11 - y) + x] != 0);
                setPixel(x, y, on & toggle ? grays[2] : black);
            }
        }
        strip.show();
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
    int nGamut = greenSpectrum.nGamut();
    uint32_t c1 = greenSpectrum.gamut()[nGamut - 1];
    uint32_t c2 = greenSpectrum.gamut()[nGamut - 2];
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
    if (isTimeout()) {
        if (random(2) == 0) {
            left(greenSpectrum.gamut()[nGamut - 1]);
        }
        else {
            right(greenSpectrum.gamut()[nGamut - 1]);
        }
        uint16_t t = (uint16_t) exponential(1000.);
        setTimeout(t);
    }
    strip.show();
    rotate(greenSpectrum, false);
    delay(50);
}

void effectMatrixFire() {
    int nGamut = planckSpectrum.nGamut();
    uint32_t c1 = planckSpectrum.gamut()[nGamut - 1];
    uint32_t c2 = planckSpectrum.gamut()[nGamut - 2];
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
    rotate(planckSpectrum, false);
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

void effectRanxels() {
    for (int i = 0; i < 8; i++) {
        ranxels(orangeSpectrum);
        top(orangeSpectrum.at(i / 8.));
        strip.show();
    }
    for (int i = 7; i >= 0; i--) {
        ranxels(orangeSpectrum);
        top(orangeSpectrum.at(i / 8.));
        strip.show();
    }
}

void effectSwipeFadingPlanckRanxels() {
    uint32_t *gamut = planckSpectrum.gamut();
    int nGamut = planckSpectrum.nGamut();
    uint32_t first = gamut[nGamut - 1];
    uint32_t last = gamut[0];
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
        rotate(planckSpectrum, false);
        strip.show();
    }
    vertical(11, last);
    for (int y = 0; y < 12; y++) {
        if (random(2) == 0) {
            setPixel(11, y, first);
        }
    }
    for (int n = 0; n < 200; n++) {
        rotate(planckSpectrum, false);
        strip.show();
    }
}

void effectFadingPlanckPixels() {
    int nGamut = planckSpectrum.nGamut();
    solid(black);
    for (int n = 0; n < 10; n++) {
        setPixel(random(210), planckSpectrum.gamut()[nGamut - 1]);
    }
    strip.show();
    for (int n = 0; n < 100; n++) {
        rotate(planckSpectrum, false);
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

void effectBlink() {
    solid(black);
    strip.show();
    solid(white);
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
        uint16_t t = (uint16_t) exponential(1000.);
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
    uint32_t *gamut = planckSpectrum.gamut();
    int nGamut = planckSpectrum.nGamut();
    for (int n = nGamut - 1; n >= 0; n--) {
        solid(gamut[n]);
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
    for (int i = 0; i < 8; i++) {
        solid(interpolate(co, cn, i / 7.));
        strip.show();
    }
}
