#include "Adafruit_WS2801.h"

#include "util.h"
#include "colors.h"

extern Adafruit_WS2801 strip;

void left(uint32_t c) {
    for (int i = LEFT_START; i <= LEFT_END; i++) {
        strip.setPixelColor(i, c);
    }
}

void right(uint32_t c) {
    for (int i = RIGHT_START; i <= RIGHT_END; i++) {
        strip.setPixelColor(i, c);
    }
}

void top(uint32_t c) {
    left(c);
    right(c);
}

void mid(uint32_t c) {
    for (int i = MID_START; i <= MID_END; i++) {
        strip.setPixelColor(i, c);
    }
}

int midLogicToIndex(int x, int y) {
    if ((y == 0) && (x < 4 || x > 7)) {
        return -1;
    }
    if ((y == 1) && (x < 2 || x > 9)) {
        return -1;
    }
    switch (y) {
        case 0:
            return 7 - x;
        case 1:
            return 2 + x;
        case 2:
            return 24 - x - 1;
        case 3:
            return 24 + x;
        case 4:
            return 48 - x - 1;
        case 5:
            return 48 + x;
        case 6:
            return 72 - x - 1;
        case 7:
            return 72 + x;
        case 8:
            return 96 - x - 1;
        case 9:
            return 96 + x;
        case 10:
            return 120 - x - 1;
        case 11:
            return 120 + x;
    }
}

int leftLogicToIndex(int x, int y) {
    switch (y) {
        case 0:
            return 134 - x;
        case 1:
            return 135 + x;
        case 2:
            return 147 - x;
        case 3:
            return 148 + x;
        case 4:
            return 161 - x;
        case 5:
            return 162 + x;
        case 6:
            return 170 - x;
    }
}

int rightLogicToIndex(int x, int y) {
    switch (y) {
        case 0:
            return 207 + x;
        case 1:
            return 206 - x;
        case 2:
            return 194 + x;
        case 3:
            return 193 - x;
        case 4:
            return 180 + x;
        case 5:
            return 179 - x;
        case 6:
            return 171 + x;
    }
}

void leftSetPixels(int pairs[][2], int n_pixels, uint32_t c) {
    for (int i = 0; i < n_pixels; i++) {
        int *pair = pairs[i];
        setPixel(leftLogicToIndex(pair[0], pair[1]), c);
    }
}

void rightSetPixels(int pairs[][2], int n_pixels, uint32_t c) {
    for (int i = 0; i < n_pixels; i++) {
        int *pair = pairs[i];
        setPixel(rightLogicToIndex(pair[0], pair[1]), c);
    }
}

int ringPairs[16][2] = {
    {0, 0}, {0, 1}, {0, 2}, {0, 3}, {0, 4}, {0, 5}, {0, 6}, {1, 6}, {2, 6},
    {5, 5}, {6, 4}, {6, 3}, {6, 2}, {5, 1}, {2, 0}, {1, 0}
};

int topRowPairs0[][2] = {{0, 0}, {1, 0}, {2, 0}};
int topRowPairs1[][2] = {{0, 1}, {1, 1}, {2, 1}, {3, 1}, {4, 1}, {5, 1}};
int topRowPairs2[][2] = {{0, 2}, {1, 2}, {2, 2}, {3, 2}, {4, 2}, {5, 2}, {6, 2}};
int topRowPairs3[][2] = {{0, 3}, {1, 3}, {2, 3}, {3, 3}, {4, 3}, {5, 3}, {6, 3}};
int topRowPairs4[][2] = {{0, 4}, {1, 4}, {2, 4}, {3, 4}, {4, 4}, {5, 4}, {6, 4}};
int topRowPairs5[][2] = {{0, 5}, {1, 5}, {2, 5}, {3, 5}, {4, 5}, {5, 5}};
int topRowPairs6[][2] = {{0, 6}, {1, 6}, {2, 6}};

void leftRow(int row, uint32_t c) {
    switch (row) {
        case 0:
            leftSetPixels(topRowPairs0, 3, c);
            return;
        case 1:
            leftSetPixels(topRowPairs1, 6, c);
            return;
        case 2:
            leftSetPixels(topRowPairs2, 7, c);
            return;
        case 3:
            leftSetPixels(topRowPairs3, 7, c);
            return;
        case 4:
            leftSetPixels(topRowPairs4, 7, c);
            return;
        case 5:
            leftSetPixels(topRowPairs5, 6, c);
            return;
        case 6:
            leftSetPixels(topRowPairs6, 3, c);
            return;
    }
}

void rightRow(int row, uint32_t c) {
    switch (row) {
        case 0:
            rightSetPixels(topRowPairs0, 3, c);
            return;
        case 1:
            rightSetPixels(topRowPairs1, 6, c);
            return;
        case 2:
            rightSetPixels(topRowPairs2, 7, c);
            return;
        case 3:
            rightSetPixels(topRowPairs3, 7, c);
            return;
        case 4:
            rightSetPixels(topRowPairs4, 7, c);
            return;
        case 5:
            rightSetPixels(topRowPairs5, 6, c);
            return;
        case 6:
            rightSetPixels(topRowPairs6, 3, c);
            return;
    }
}

void leftRing(uint32_t c) {
    leftSetPixels(ringPairs, 16, c);
}

void rightRing(uint32_t c) {
    rightSetPixels(ringPairs, 16, c);
}

void shiftDown() {
    for (int y = 0; y < 11; y++) {
        for (int x = 0; x < 12; x++) {
            int i = midLogicToIndex(x, y + 1);
            uint32_t c = strip.getPixelColor(i);
            i = midLogicToIndex(x, y);
            strip.setPixelColor(i, c);
        }
    }
}

void shiftLeft() {
    for (int x = 0; x < 11; x++) {
        for (int y = 2; y < 12; y++) {
            int i = midLogicToIndex(x + 1, y);
            uint32_t c = strip.getPixelColor(i);
            i = midLogicToIndex(x, y);
            strip.setPixelColor(i, c);
        }
    }
}

void dim() {
    for (int i = 0; i < strip.numPixels(); i++) {
        uint32_t c = strip.getPixelColor(i);
        c = pixelDim(c);
        strip.setPixelColor(i, c);
    }
}

void dimLess() {
    for (int i = 0; i < strip.numPixels(); i++) {
        uint32_t c = strip.getPixelColor(i);
        c = pixelDimLess(c);
        strip.setPixelColor(i, c);
    }
}

void dimFloat(float f) {
    for (int r = 0; r < strip.numPixels(); r++) {
        uint32_t c = strip.getPixelColor(r);
        c = pixelDimFloat(c, f);
        strip.setPixelColor(r, c);
    }
}

void topDim(float f) {
    for (int r = LEFT_START; r <= RIGHT_END; r++) {
        uint32_t c = strip.getPixelColor(r);
        c = pixelDimFloat(c, f);
        strip.setPixelColor(r, c);
    }
}

void midDim() {
    for (int r = MID_START; r <= MID_END; r++) {
        uint32_t c = strip.getPixelColor(r);
        c = pixelDim(c);
        strip.setPixelColor(r, c);
    }
}

void midDimLess() {
    for (int r = MID_START; r <= MID_END; r++) {
        uint32_t c = strip.getPixelColor(r);
        c = pixelDimLess(c);
        strip.setPixelColor(r, c);
    }
}

void midDimFloat(float f) {
    for (int r = MID_START; r <= MID_END; r++) {
        uint32_t c = strip.getPixelColor(r);
        c = pixelDimFloat(c, f);
        strip.setPixelColor(r, c);
    }
}

uint32_t pixelDim(uint32_t c) {
    int r = (c & 0x00FF0000) >> 17;
    int g = (c & 0x0000FF00) >> 9;
    int b = (c & 0x000000FF) >> 1;
    return color(r, g, b);
}

uint32_t pixelDimLess(uint32_t c) {
    int r = (c & 0x00FF0000) >> 17;
    int g = (c & 0x0000FF00) >> 9;
    int b = (c & 0x000000FF) >> 1;
    r += (r >> 1);
    g += (g >> 1);
    b += (b >> 1);
    return color(r, g, b);
}

uint32_t pixelDimFloat(uint32_t c, float f) {
    int r = (int) (((c & 0x00FF0000) >> 16) * f);
    int g = (int) (((c & 0x0000FF00) >>  8) * f);
    int b = (int) (((c & 0x000000FF) >>  0) * f);
    return color(r, g, b);
}

void horizontal(int row, uint32_t c) {
    for (int x = 0; x < 12; x++) {
        int i = midLogicToIndex(x, row);
        strip.setPixelColor(i, c);
    }
}

void vertical(int col, uint32_t c) {
    for (int y = 0; y < 12; y++) {
        int i = midLogicToIndex(col, y);
        strip.setPixelColor(i, c);
    }
}

// The size should be between 1 and 6 inclusive.
void square(int size, uint32_t c) {
    int left = 6 - size;
    int bottom = 6 - size;
    int top = 5 + size;
    int right = 5 + size;
    for (int x=left; x<=right; x++) {
        int r = midLogicToIndex(x, top);
        strip.setPixelColor(r, c);
        r = midLogicToIndex(x, bottom);
        strip.setPixelColor(r, c);
    }
    for (int y = bottom; y <= top; y++) {
        int r = midLogicToIndex(left, y);
        strip.setPixelColor(r, c);
        r = midLogicToIndex(right, y);
        strip.setPixelColor(r, c);
    }
}

void ranxels(Spectrum &s) {
    for (int i = 0; i < strip.numPixels(); i++) {
        uint32_t c = ranxel(s);
        strip.setPixelColor(i, c);
    }
}

uint32_t ranxel(Spectrum &s) {
    return s.at(random(100) / 100.);
}

uint32_t ranxel() {
    byte r = (byte) (1 << random(8));
    byte g = (byte) (1 << random(8));
    byte b = (byte) (1 << random(8));
    return color(r, g, b);
}

void solid(uint32_t c) {
    for (int i = 0; i < strip.numPixels(); i++) {
        strip.setPixelColor(i, c);
    }
}

void gradient(uint32_t cl, uint32_t ch) {
    for (int x = 0; x < 12; x++) {
        float w = x / 11.;
        uint32_t c = interpolate(cl, ch, w);
        vertical(x, c);
    }
}

void gradient(Spectrum &s) {
    for (int x = 0; x < 12; x++) {
        float w = x / 11.;
        uint32_t c = s.at(w);
        vertical(x, c);
    }
}

void midSolid(uint32_t c) {
    for (int i = MID_START; i <= MID_END; i++) {
        strip.setPixelColor(i, c);
    }
}

// Positions cycle from red to green to blue and back to red.
uint32_t wheel(byte pos) {
    if (pos < 85) {
        return color(pos * 3, 255 - pos * 3, 0);
    }
    else if (pos < 170) {
        pos -= 85;
        return color(255 - pos * 3, 0, pos * 3);
    }
    else {
        pos -= 170; 
        return color(0, pos * 3, 255 - pos * 3);
    }
}

uint32_t getPixel(int i) {
    return strip.getPixelColor(i);
}

void setPixel(int i, uint32_t c) {
    strip.setPixelColor(i, c);
}

void setPixels(int *indices, int n_indices, uint32_t c) {
    for (int i = 0; i < n_indices; i++) {
        setPixel(indices[i], c);
    }
}

void setPixel(int x, int y, uint32_t c) {
    int i = midLogicToIndex(x, y);
    strip.setPixelColor(i, c);
}

// Return a sample value of an exponentially distributed random variable.
float exponential(float lambda) {
    long r = random(100);
    return - lambda * log(1 - (r / 100.));
}

uint32_t decay(uint32_t c) {
    return pixelDimFloat(c, .9);
}

unsigned long expiry = 0;
bool isTimeoutSet = true;

void setTimeout(unsigned long m) {
    expiry = millis() + m;
    isTimeoutSet = true;
}

bool isTimeout() {
    return isTimeoutSet && millis() >= expiry;
}

void clearTimeout() {
    isTimeoutSet = false;
}

void rotate(Spectrum &s, int i, bool wrap) {
    uint32_t c = strip.getPixelColor(i);
    c = s.prev(c, wrap);
    strip.setPixelColor(i, c);
}

void rotate(Spectrum &s, bool wrap) {
    for (int i = 0; i < strip.numPixels(); i++) {
        rotate(s, i, wrap);
    }
}
