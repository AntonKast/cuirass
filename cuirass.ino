#include <SPI.h>
#include <Adafruit_WS2801.h>

#include "util.h"
#include "colors.h"
#include "spectrum.h"

// initialize strip

byte dataPin  = 2;    // yellow wire
byte clockPin = 3;    // green wire

byte numPixels = 210;

Adafruit_WS2801 strip = Adafruit_WS2801(numPixels, dataPin, clockPin);

byte initSkip;

int nshift = 0;

void updateShift() {
    nshift = analogRead(0) >> 7;
}

uint32_t shift(uint32_t c) {
    int r = c >> 16 & 0xFF;
    int g = c >>  8 & 0xFF;
    int b = c >>  0 & 0xFF;
    return color(r >> nshift, g >> nshift, b >> nshift);
}

void setup() {    
    strip.begin();
    strip.show();
    randomSeed(analogRead(0));
    initSkip = random(256);
    updateShift();
}

void fadeToBlack() {
    for (byte i = 0; i < 100; i++) {
        dimFloat(.95);
        strip.show();
    }
}

void repeat(void effect(), int n) {
    if (initSkip > 0) {
        initSkip--;
        return;
    }
    for (int i = 0; i < n; i++) {
        effect();
    }
    fadeToBlack();
}

void loop() {
    repeat(effectWave, 200);
    repeat(effectBurst, 3000);
    repeat(effectFadingRanxels, 4800);
    repeat(effectPolkaDots, 1);
    repeat(effectMatrix, 1600);
    repeat(effectCrawlTextFear, 10);
    repeat(effectMouth, 430);
    repeat(effectFire, 80);
    repeat(effectRanxels, 3840);
    repeat(effectCrawlTextLust, 10);
    repeat(effectRainbow, 15);
    repeat(effectCrazyColors, 480);
    repeat(effectRedWhiteBlue, 3000);
    repeat(effectSeizureProgressive, 1);
    repeat(effectCrawlTextLove, 10);
    repeat(effectFlare, 13);
    repeat(effectChecker, 240);
    repeat(effectFlicker, 4800);
    repeat(effectRainbowFrame, 1);
    repeat(effectIrisLolaText, 10);
    repeat(effectFireworks, 12);
}

// effects

void effectFireworks() {
    float r = 1.;
    byte x = random(12);
    byte y = random(12);
    for (float h = 0.; h < y; h+= .1) {
        solid(black);
        for (byte i = 0; i < 12; i++) {
            for (byte j = 0; j < 12; j++) {
                float dx = x - i;
                float dy = h - j;
                float d = sqrt(dx * dx + dy * dy);
                uint32_t c = interpolate(white, black, constrain(d / r, 0., 1.));
                setPixel(i, j, c);
            }
        }
        strip.show();
    }
    byte nGamut = 100;
    Spectrum s = createPlanckSpectrum(nGamut);
    uint32_t *gamut = s.gamut;
    uint32_t max = gamut[nGamut - 1];
    for (r = 1.; r <= 8.; r += 1.) {
        for (byte i = 0; i < 12; i++) {
            for (byte j = 0; j < 12; j++) {
                float dx = x - i;
                float dy = y - j;
                float d = sqrt(dx * dx + dy * dy);
                uint32_t c = interpolate(max, black, constrain(d / r, 0., 1.));
                setPixel(i, j, c);
            }
        }
        strip.show();
    }
    solid(max);
    for (r = 1.; r <= 10.; r += .5) {
        for (byte i = 0; i < 12; i++) {
            for (byte j = 0; j < 12; j++) {
                float dx = x - i;
                float dy = y - j;
                float d = sqrt(dx * dx + dy * dy);
                if (d < r) {
                    rotate(s, midLogicToIndex(i, j), false);
                }
            }
        }
        strip.show();
    }
    for (byte n = 0; n < nGamut; n++) {
        rotate(s, false);
        strip.show();
    }
    destroySpectrum(s);
}

// shifted
void effectWave() {
    for (float p = 0.; p < 6.28; p += .4) {
        updateShift();
        for (int i = 0; i < 12; i++) {
            for (int j = 0; j < 12; j++) {
                float r = 6.28 * sqrt(sq(i - 5.5) + sq(j - 5.5)) / 5.5;
                uint8_t level = constrain(128 * (1 + sin(r - p)), 0, 255);
                uint32_t c = shift(gray(level));
                setPixel(i, j, c);
            }
        }
        float r = p;
        uint8_t level = constrain(128 * (1 + sin(r)), 0, 255);
        uint32_t c = shift(gray(level));
        topCenter(c);

        r -= 3.14 / 8 ;
        level = constrain(128 * (1 + sin(r)), 0, 255);
        c = shift(gray(level));
        topInnerRing(c);

        r -= 3.14 / 8 ;
        level = constrain(128 * (1 + sin(r)), 0, 255);
        c = shift(gray(level));
        topMidRing(c);

        r -= 3.14 / 8 ;
        level = constrain(128 * (1 + sin(r)), 0, 255);
        c = shift(gray(level));
        topOuterRing(c);

        r -= 3.14 / 8 ;
        level = constrain(128 * (1 + sin(r)), 0, 255);
        c = shift(gray(level));
        topEdge(c);

        strip.show();
    }
}

void effectFire() {
    uint32_t orange = interpolate(green, red, 5 / 6.);
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
    uint32_t back, front;
    int nReps = 10;
    
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

Spectrum createRainbowSpectrum() {
    uint32_t rainbowColors[] = {
        red,
        green,
        blue,
        interpolate(blue, red, .99),
        termColor
    };
    return createSpectrum(rainbowColors, 40);
}

void effectRainbowFrame() {
    byte aText[] = {
        0, 0, 0, 0, 1, 1, 0, 0, 0, 0,
        0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 
        0, 0, 0, 1, 0, 0, 1, 0, 0, 0,
        0, 0, 0, 1, 1, 1, 1, 0, 0, 0,
        0, 0, 0, 1, 0, 0, 1, 0, 0, 0,
        0, 0, 0, 1, 0, 0, 1, 0, 0, 0,
        0, 0, 0, 1, 0, 0, 1, 0, 0, 0,
    };

    byte kText[] = {
        0, 0, 0, 1, 0, 0, 1, 0, 0, 0,
        0, 0, 0, 1, 0, 0, 1, 0, 0, 0,
        0, 0, 0, 1, 0, 1, 0, 0, 0, 0,
        0, 0, 0, 1, 1, 0, 0, 0, 0, 0,
        0, 0, 0, 1, 0, 1, 0, 0, 0, 0,
        0, 0, 0, 1, 0, 0, 1, 0, 0, 0,
        0, 0, 0, 1, 0, 0, 1, 0, 0, 0,
    };
    Spectrum s = createRainbowSpectrum();
    uint32_t *gamut = s.gamut;

    solid(black);
    byte n = 0;
    for (byte x = 0; x < 12; x++) {
        setPixel(x, 2, gamut[n++]);
    }
    for (byte y = 3; y < 12; y++) {
        setPixel(11, y, gamut[n++]);
    }
    for (byte x = 10; x < 255; x--) {
        setPixel(x, 11, gamut[n++]);
    }
    for (byte y = 10; y > 2; y--) {
        setPixel(0, y, gamut[n++]);
    }
    n = 0;
    for (byte i = 0; i < 16; i++) {
        uint8_t *pair = &outerRingPairs[2 * i];
        setPixel(leftLogicToIndex(pair[0], pair[1]), gamut[n++]);
        setPixel(rightLogicToIndex(pair[0], pair[1]), gamut[n++]);
    }
    uint32_t gray2 = graylevel(2);

    int steps = 8000;
    for (int m = 0; m < steps; m++) {

        for (int x = 0; x < 12; x++) {
            rotate(s, midLogicToIndex(x, 2), true);
        }
        for (int y = 3; y < 12; y++) {
            rotate(s, midLogicToIndex(11, y), true);
        }
        for (int x = 10; x >= 0; x--) {
            rotate(s, midLogicToIndex(x, 11), true);
        }
        for (int y = 10; y > 2; y--) {
            rotate(s, midLogicToIndex(0, y), true);
        }
        for (int i = 0; i < 16; i++) {
            uint8_t *pair = &outerRingPairs[2 * i];
            rotate(s, leftLogicToIndex(pair[0], pair[1]), true);
            rotate(s, rightLogicToIndex(pair[0], pair[1]), true);
        }
        bool toggle = m % 2 == 0;
        bool ak = (m / 20) % 2 == 0;

        for (byte x = 1; x < 11; x++) {
            for (byte y = 3; y < 10; y++) {
                bool on = ak ?
                    (aText[10 * (9 - y) + (x - 1)] != 0) : 
                    (kText[10 * (9 - y) + (x - 1)] != 0);
                setPixel(x, y, on & toggle ? gray2 : black);
            }
        }
        strip.show();
    }
    destroySpectrum(s);
}

void effectIrisLolaText() {
    byte irisText[] = {
        0, 1, 0, 1, 1, 0, 0, 1, 0, 0, 1, 0,
        0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1,
        0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0,
        0, 1, 0, 1, 1, 0, 0, 1, 0, 0, 1, 0,
        0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1,
        0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 
        0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 1, 0,
    };              
                    
    byte lolaText[] = { 
        1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 1, 0,
        1, 0, 1, 0, 1, 0, 1, 0, 0, 1, 0, 1,
        1, 0, 1, 0, 1, 0, 1, 0, 0, 1, 0, 1,
        1, 0, 1, 0, 1, 0, 1, 0, 0, 1, 1, 1,
        1, 0, 1, 0, 1, 0, 1, 0, 0, 1, 0, 1,
        1, 0, 1, 0, 1, 0, 1, 0, 0, 1, 0, 1,
        1, 1, 0, 1, 0, 0, 1, 1, 0, 1, 0, 1,
    };  
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

void crawlText(byte text[]) {
    solid(black);
    uint32_t c = graylevel(2);
    for (int i = 0; i < 15; i++) {
        shiftLeft();
        for (int y = 3; y < 10; y++) {
            if (text[15 * (9 - y) + i] > 0) {
                setPixel(11, y, c);
            }
            else {
                setPixel(11, y, black);
            }
        }
        strip.show();
        delay(100);
    }
    for (int i = 0; i < 12; i++) {
        shiftLeft();
        vertical(11, black);
        strip.show();
        delay(100);
    }
}

void effectCrawlTextFear() {
    byte text[] = { 
        1, 1, 1, 0, 1, 1, 1, 0, 0, 1, 0, 0, 1, 1, 0,
        1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1,
        1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1,
        1, 1, 1, 0, 1, 1, 0, 0, 1, 1, 1, 0, 1, 1, 0,
        1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1,
        1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1,
        1, 0, 0, 0, 1, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1,
    };          
    crawlText(text);
}

void effectCrawlTextLust() {
    byte text[] = {
        1, 0, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 1, 1, 1,
        1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 1, 0,
        1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 1, 0,
        1, 0, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 0, 1, 0,
        1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0,
        1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 1, 0,
        1, 1, 1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0,
    };
    crawlText(text);
}

void effectCrawlTextLove() {
    byte text[] = {
        1, 0, 0, 0, 0, 1, 0, 0, 1, 0, 1, 0, 1, 1, 1,
        1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0,
        1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0,
        1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 0,
        1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0,
        1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0,
        1, 1, 1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 1, 1,
    };
    crawlText(text);
}

Spectrum createMatrixSpectrum() {
    uint32_t matrixColors[] = {
        black,
        interpolate(black, green, .02),
        interpolate(black, green, .1),
        green,
        termColor
    };
    return createSpectrum(matrixColors, 10);
}

void effectMatrix() {
    Spectrum s = createMatrixSpectrum();
    int nGamut = s.size;
    uint32_t *gamut = s.gamut;

    uint32_t c1 = gamut[nGamut - 1];
    uint32_t c2 = gamut[nGamut - 2];
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
            left(gamut[nGamut - 1]);
        }
        else {
            right(gamut[nGamut - 1]);
        }
        uint16_t t = (uint16_t) exponential(1000.);
        setTimeout(t);
    }
    strip.show();
    rotate(s, false);
    delay(50);

    destroySpectrum(s);
}

Spectrum createPlanckSpectrum(byte size) {
    uint32_t planckColors[] = {
        black,
        interpolate(black, red, .1),
        interpolate(black, red, .2),
        interpolate(black, red, .5),
        interpolate(green, red, 5 / 6.), // magenta
        interpolate(red, green, .5),     // yellow
        white,
        termColor
    };
    return createSpectrum(planckColors, size);
}

void effectBurst() {

    byte nShort = 40;
    Spectrum sShort = createPlanckSpectrum(nShort);

    byte nLong = 100;
    Spectrum sLong = createPlanckSpectrum(nLong);

    uint32_t first = sShort.gamut[nShort - 1];
    uint32_t second = sShort.gamut[nShort - 2];
    for (byte x = 0; x < 12; x++) {
        for (byte y = 11; y > 0; y--) {
            byte i = midLogicToIndex(x, y);
            uint32_t c = getPixel(i);
            if (c == second) {
                setPixel(x, y - 1, first);
            }
        }
    }

    if (isTimeout()) {
        top(sLong.gamut[nLong - 1]);

        uint32_t c = sShort.gamut[nShort - 1];
        for (byte x = 0; x < 12; x++) {
            setPixel(x, 11, c);
        }
        uint16_t t = (uint16_t) exponential(5000.);
        setTimeout(t);
    }
    strip.show();

    rotateMid(sShort, false);
    rotateTop(sLong, false);

    destroySpectrum(sShort);
    destroySpectrum(sLong);
}

// shifted
void effectRedWhiteBlue() {
    updateShift();

    uint32_t r = shift(red);
    uint32_t w = shift(white);
    uint32_t b = shift(blue);

    for (int i = LEFT_START; i <= LEFT_END; i++) {
        uint32_t c = getPixel(i);
        if (c != b) {
            c = interpolate(c, b, .1);
            setPixel(i, c);
        }
    }
    for (int i = RIGHT_START; i <= RIGHT_END; i++) {
        uint32_t c = getPixel(i);
        if (c != b) {
            c = interpolate(c, b, .1);
            setPixel(i, c);
        }
    }
    int il = random(LEFT_END - LEFT_START + 1) + LEFT_START;
    int ir = random(RIGHT_END - RIGHT_START + 1) + RIGHT_START;
    strip.setPixelColor(il, w);
    strip.setPixelColor(ir, w);

    uint32_t g = shift(graylevel(2));
    g = max(g, gray(1));

    horizontal( 0, r);
    horizontal( 1, g);
    horizontal( 2, r);
    horizontal( 3, g);
    horizontal( 4, r);
    horizontal( 5, g);
    horizontal( 6, r);
    horizontal( 7, g);
    horizontal( 8, r);
    horizontal( 9, g);
    horizontal(10, r);
    horizontal(11, g);

    strip.show();
}

void effectRanxels() {
    uint32_t c = interpolate(red, green, .5);
    for (int i = 0; i < numPixels; i++) {
        setPixel(i, pixelDimExp(c, random(7)));
    }
    strip.show();
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

// shifted
void effectSeizureProgressive() {
    int nReps = 128;
    for (int p = 2; p <= 12; p++) {
        for (int n = 0; n < nReps / p; n++) {
            updateShift();
            uint32_t gray2 = shift(graylevel(2));
            if (gray2 == 0) {
                gray2 = gray(0x01);
            }
            for (int o = 0; o < p; o++) {
                solid(black);
                for (int i = o; i < numPixels; i += p) {
                    setPixel(i, gray2);
                }
                strip.show();
            }
        }
    }
    for (int p = 12; p >= 2; p--) {
        for (int n = 0; n < nReps / p; n++) {
            updateShift();
            uint32_t gray2 = shift(graylevel(2));
            if (gray2 == 0) {
                gray2 = gray(0x01);
            }
            for (int o = 0; o < p; o++) {
                solid(black);
                for (int i = o; i < numPixels; i += p) {
                    setPixel(i, gray2);
                }
                strip.show();
            }
        }
    }
}

// shifted
void effectChecker() {
    updateShift();
    solid(black);
    uint32_t gray2 = shift(graylevel(2));
    if (gray2 == 0) {
        gray2 = gray(0x01);
    }
    for (int i = 0; i < numPixels; i += 2) {
        setPixel(i, gray2);
    }
    strip.show();
    delay(200);
    updateShift();
    solid(black);
    gray2 = shift(graylevel(2));
    if (gray2 == 0) {
        gray2 = gray(0x01);
    }
    for (int i = 1; i < numPixels; i += 2) {
        setPixel(i, gray2);
    }
    strip.show();
    delay(200);
}

void effectFlicker() {
    if (isTimeout()) {
        solid(color(255, 32, 0));
        uint16_t t = (uint16_t) exponential(1000.);
        setTimeout(t);
    }
    dimFloat(.8);
    strip.show();
}

// shifted
void effectFadingRanxels() {
    dimFloat(.99);
    setPixel(random(210), shift(blue));
    strip.show();
}

void effectFlare() {
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

// shifted
void effectRainbow() {
    for (int j = 0; j < 256; j++) {
        updateShift();
        for (int i = 0; i < strip.numPixels(); i++) {
            strip.setPixelColor(i, shift(wheel((i + j) % 255)));
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
