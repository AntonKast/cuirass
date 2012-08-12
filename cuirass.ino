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

// spectra

uint32_t planckColors[] = {
    black,
    interpolate(black, red, .1),
    interpolate(black, red, .2),
    interpolate(black, red, .5),
    interpolate(green, red, 5 / 6.), // magenta
    interpolate(red, green, .5),     // yellow
    0x00ffffff,                      // white
    termColor
};
Spectrum planckSpectrum = Spectrum(planckColors, 100);
Spectrum planckSpectrumShort = Spectrum(planckColors, 40);

uint32_t matrixColors[] = {
    black,
    interpolate(black, green, .02),
    interpolate(black, green, .1),
    green,
    termColor
};
Spectrum matrixSpectrum = Spectrum(matrixColors, 10);

uint32_t magenta = interpolate(blue,  red,   .5);
uint32_t ranxelColors[] = {
    black,
    interpolate(black, magenta, .1),
    interpolate(black, magenta, .2),
    magenta,
    termColor
};
Spectrum ranxelSpectrum = Spectrum(ranxelColors, 0);

uint32_t rainbowColors[] = {
    red,
    green,
    blue,
    interpolate(blue, red, .99),
    termColor
};
Spectrum rainbowSpectrum = Spectrum(rainbowColors, 40);

// main loop

void setup() {    
    strip.begin();
    strip.show();
}

void repeat(void f(), int n) {
    int div = 1;    // cycle faster in development
    for (int i = 0; i <= n / div; i++) {
        f();
    }
    for (int i = 0; i < 100; i++) {
        dimFloat(.95);
        strip.show();
    }
}

void loop() {

//    repeat(effectRainbow, 15);        // 2 minutes
//    repeat(effectCrazyColors, 480);   // 2 minutes
//    repeat(effectMatrix, 1600);       // 2 minutes
//    repeat(effectRedWhiteBlue, 3000); // 2 minutes
//    repeat(effectPolkaDots, 2);       // 2 minutes 40 seconds
//    repeat(effectFireworks, 900);     // 2 minutes
//    repeat(effectRanxels, 30);        // 2 minutes
//    repeat(effectMouth, 430);         // 2 minutes
//    repeat(effectFire, 80);           // 2 minutes
//    repeat(effectSeizure, 40000);     // 2 minutes
//    repeat(effectFlash, 13);          // 2 minutes
//    repeat(effectSignature, 10);      // 13 seconds
//    repeat(effectRainbowFrame, 1);    // 2 minutes
//    repeat(effectChecker, 240);       // 2 minutes
//    repeat(effectFadingRanxels, 4800);// 2 minutes
//    repeat(effectFlare, 4800);        // 2 minutes

    strip.show();

//    repeat(effectCrawlText, 10); // broken

//    effectIrisLolaText();     // for production?
//    effectSignature();        // for production?

//    effectBlinkText();
//    effectPlanckFlash();
//    effectSwipeFadingPlanckRanxels();
}

// effects

void effectFire() {
    uint32_t orange = interpolate(green, red, .5);
    for (int row = 0; row < 42; row++) {
        solid(black);
        leftRow(row % 7, orange);
        rightRow(row % 7, orange);
        horizontal(row % 12, orange);
        horizontal((row + 6) % 12, orange);
        strip.show();
    }
}

void effectPolkaDots() {
    int nReps = 10;

    uint32_t back, front;

    back = interpolate(black, red, .25);
    front = white;
    for (int n = 0; n < 16; n++) {
        fadeTowards(back, .1);
        strip.show();
    }
    for (int n = 0; n < nReps; n++) {
        polkaDotsLoop(back, front);
    }

    back = interpolate(black, blue, .25);
    front = green;
    for (int n = 0; n < 16; n++) {
        fadeTowards(back, .1);
        strip.show();
    }
    for (int n = 0; n < nReps; n++) {
        polkaDotsLoop(back, front);
    }

    uint32_t yellow  = interpolate(red,   green, .5);
    back = interpolate(black, yellow, .25);
    front = red;
    for (int n = 0; n < 16; n++) {
        fadeTowards(back, .1);
        strip.show();
    }
    for (int n = 0; n < nReps; n++) {
        polkaDotsLoop(back, front);
    }
}

void polkaDotsLoop(uint32_t back, uint32_t front) {
    solid(back);
    strip.show();

    int nPts = 30;
    uint32_t pts[nPts];
    for (int i = 0; i < nPts; i++) {
        pts[i] = random(numPixels);
    }
    int nSteps = 32;
    for (int n = 0; n < nSteps; n++) {
        for (int i = 0; i < nPts; i++) {
            setPixel(pts[i], interpolate(back, front, n / (float) nSteps));
        }
        strip.show();
    }
    delay(400);
    for (int n = nSteps - 1; n >= 0; n--) {
        for (int i = 0; i < nPts; i++) {
            setPixel(pts[i], interpolate(back, front, n / (float) nSteps));
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
        uint8_t *pair = &outerRingPairs[2 * i];
        setPixel(leftLogicToIndex(pair[0], pair[1]), gamut[n++]);
        setPixel(rightLogicToIndex(pair[0], pair[1]), gamut[n++]);
    }
    for (int m = 0; m < 4000; m++) {

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
            uint8_t *pair = &outerRingPairs[2 * i];
            rotate(rainbowSpectrum, leftLogicToIndex(pair[0], pair[1]), true);
            rotate(rainbowSpectrum, rightLogicToIndex(pair[0], pair[1]), true);
        }
        strip.show();
    }
}

void effectSignature() {
    solid(black);

    for (int n = 0; n < 40; n++) {

        bool toggle = n % 2 == 0;
        bool ak = (n / 20) % 2 == 0;

        uint32_t gray2 = graylevel(2);
        for (int x = 1; x < 11; x++) {
            for (int y = 3; y < 11; y++) {
                bool on = ak ?
                    (aText[10 * (10 - y) + (x - 1)] != 0) : 
                    (kText[10 * (10 - y) + (x - 1)] != 0);
                setPixel(x, y, on & toggle ? gray2 : black);
            }
        }
        strip.show();
    }
}

void effectIrisLolaText() {
    solid(black);
    for (int n = 0; n <= 64; n++) {
        uint32_t c = interpolate(black, white, n / 64.);
        for (int x = 0; x < 12; x++) {
            for (int y = 3; y < 10; y++) {
                if (irisText[12 * (9 - y) + x] > 0) {
                    setPixel(x, y, c);
                }
            }
        }
        strip.show();
    }
    for (int n = 64; n >= 0; n--) {
        uint32_t c = interpolate(black, white, n / 64.);
        for (int x = 0; x < 12; x++) {
            for (int y = 3; y < 10; y++) {
                if (irisText[12 * (9 - y) + x] > 0) {
                    setPixel(x, y, c);
                }
            }
        }
        strip.show();
    }
    solid(black);
    for (int n = 0; n <= 64; n++) {
        uint32_t c = interpolate(black, white, n / 64.);
        for (int x = 0; x < 12; x++) {
            for (int y = 3; y < 10; y++) {
                if (lolaText[12 * (9 - y) + x] > 0) {
                    setPixel(x, y, c);
                }
            }
        }
        strip.show();
    }
    for (int n = 64; n >= 0; n--) {
        uint32_t c = interpolate(black, white, n / 64.);
        for (int x = 0; x < 12; x++) {
            for (int y = 3; y < 10; y++) {
                if (lolaText[12 * (9 - y) + x] > 0) {
                    setPixel(x, y, c);
                }
            }
        }
        strip.show();
    }
}

void effectCrawlText() {
    horizontal(0, graylevel(2));
    horizontal(1, graylevel(2));
    int row = 0;
    for (int i = 0; i < 20; i++) {
        shiftLeft();
        for (int y = 0; y < 12; y++) {
            if (fearText[20 * y + i] > 0) {
                setPixel(11, 11 - y, black);
            }
            else {
                setPixel(11, 11 - y, graylevel(2));
            }
        }
        top(graylevel(2));
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
                setPixel(x, y, on & toggle ? graylevel(2) : black);
            }
        }
        strip.show();
    }
    for (int n = 0; n < 30; n++) {
        bool toggle = n % 2 == 0;
        for (int x = 0; x < 12; x++) {
            for (int y = 2; y < 12; y++) {
                bool on = (kText[12 * (11 - y) + x] != 0);
                setPixel(x, y, on & toggle ? graylevel(2) : black);
            }
        }
        strip.show();
    }
}

void effectMatrix() {
    int nGamut = matrixSpectrum.nGamut();
    uint32_t c1 = matrixSpectrum.gamut()[nGamut - 1];
    uint32_t c2 = matrixSpectrum.gamut()[nGamut - 2];
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
            left(matrixSpectrum.gamut()[nGamut - 1]);
        }
        else {
            right(matrixSpectrum.gamut()[nGamut - 1]);
        }
        uint16_t t = (uint16_t) exponential(1000.);
        setTimeout(t);
    }
    strip.show();
    rotate(matrixSpectrum, false);
    delay(50);
}

void effectFireworks() {
    int nGamut = planckSpectrumShort.nGamut();
    uint32_t *gamut = planckSpectrumShort.gamut();

    uint32_t first = gamut[nGamut - 1];
    uint32_t second = gamut[nGamut - 2];
    for (int x = 0; x < 12; x++) {
        for (int y = 11; y > 0; y--) {
            int i = midLogicToIndex(x, y);
            uint32_t c = getPixel(i);
            if (c == second) {
                setPixel(x, y - 1, first);
            }
        }
    }
    if (isTimeout()) {
        nGamut = planckSpectrum.nGamut();
        gamut = planckSpectrum.gamut();
        top(gamut[nGamut - 1]);

        nGamut = planckSpectrumShort.nGamut();
        gamut = planckSpectrumShort.gamut();
        uint32_t c = gamut[nGamut - 1];
        for (int x = 0; x < 12; x++) {
            setPixel(x, 11, c);
        }
        uint16_t t = (uint16_t) exponential(5000.);
        setTimeout(t);
    }
    strip.show();
    rotateMid(planckSpectrumShort, false);
    rotateTop(planckSpectrum, false);
}

void effectRedWhiteBlue() {
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
    horizontal(1, graylevel(2));
    horizontal(2, red);
    horizontal(3, graylevel(2));
    horizontal(4, red);
    horizontal(5, graylevel(2));
    horizontal(6, red);
    horizontal(7, graylevel(2));
    horizontal(8, red);
    horizontal(9, graylevel(2));
    horizontal(10, red);
    horizontal(11, graylevel(2));

    strip.show();
}

void effectRanxels() {
    int nSteps = 16;
    for (int i = 0; i < nSteps; i++) {
        ranxels(ranxelSpectrum);
        top(ranxelSpectrum.at(i / (float) nSteps));
        strip.show();
    }
    for (int i = nSteps - 1; i >= 0; i--) {
        ranxels(ranxelSpectrum);
        top(ranxelSpectrum.at(i / (float) nSteps));
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

void effectMouth() {
    uint32_t c = graylevel(2);

    solid(black);
    topOuterRing(c);
    horizontal(5, c);
    horizontal(6, c);
    strip.show();

    solid(black);
    topOuterRing(c);
    horizontal(4, c);
    horizontal(7, c);
    strip.show();

    solid(black);
    topMidRing(c);
    horizontal(3, c);
    horizontal(8, c);
    strip.show();

    solid(black);
    topMidRing(c);
    horizontal(2, c);
    horizontal(9, c);
    strip.show();

    solid(black);
    topInnerRing(c);
    horizontal(1, c);
    horizontal(10, c);
    strip.show();

    solid(black);
    topCenter(c);
    horizontal(0, c);
    horizontal(11, c);
    strip.show();

    solid(black);
    topInnerRing(c);
    horizontal(1, c);
    horizontal(10, c);
    strip.show();

    solid(black);
    topInnerRing(c);
    horizontal(2, c);
    horizontal(9, c);
    strip.show();

    solid(black);
    topMidRing(c);
    horizontal(3, c);
    horizontal(8, c);
    strip.show();

    solid(black);
    topMidRing(c);
    horizontal(4, c);
    horizontal(7, c);
    strip.show();
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
    uint32_t gray2 = graylevel(2);
    solid(black);
    for (int i = 0; i < numPixels; i += 2) {
        setPixel(i, gray2);
    }
    strip.show();
    delay(200);
    solid(black);
    for (int i = 1; i < numPixels; i += 2) {
        setPixel(i, gray2);
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
    delay(exponential(3000.));
    solid(white);
    strip.show();
    delay(300);
    uint32_t c = strip.getPixelColor(0);
    while (c != red) {
        fadeTowards(red, .05);
        c = strip.getPixelColor(0);
        strip.show();
    }
    while (c > 0) {
        fadeTowards(black, .05);
        c = strip.getPixelColor(0);
        strip.show();
    }
}

void effectRainbow() {
    for (int j = 0; j < 256; j++) {
        for (int i = 0; i < strip.numPixels(); i++) {
            strip.setPixelColor(i, wheel((i + j) % 255));
        }
        strip.show();
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
