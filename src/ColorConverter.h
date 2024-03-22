#ifndef COLOR_CONVERTER_H
#define COLOR_CONVERTER_H

#include "Arduino.h"

static uint32_t Color(uint8_t r, uint8_t g, uint8_t b) {
    return ((uint32_t)r << 16) | ((uint32_t)g << 8) | b;
}

static uint16_t Color16(uint8_t r, uint8_t g, uint8_t b) {
    uint8_t red = (r >> 3)& 0x1F;
    uint8_t green = (g >> 2)& 0x3F;
    uint8_t blue = (b >> 3)& 0x1F; 
    return ((uint16_t)red << 11) | ((uint32_t)green << 5) | blue;
}

static uint16_t Color16(uint32_t color) {
    uint8_t red = (color >> 19) & 0x1F; // 16+3
    uint8_t green = (color>>10) & 0x3F; // 8+2
    uint8_t blue = (color>>3) & 0x1F;  // 3
    
    return ((uint16_t)red << 11) | ((uint16_t)green << 5) | blue;
}


uint16_t HexColor(uint8_t r, uint8_t g, uint8_t b) {
  return ((uint16_t)(r & 0xF8) << 8) | ((uint16_t)(g & 0xFC) << 3) | (b >> 3);
}


// C++ implementation of HSL to RGB conversion
// Algorithm source: https://en.wikipedia.org/wiki/HSL_and_HSV#HSL_to_RGB_alternative
static uint32_t hsl2RGB (float h, float s, float l){
    uint8_t r,g,b;

    float c = (1 - abs(2 * l - 1)) * s;
    float x = c * (1 - abs(fmod(h / 60.0, 2) - 1));
    float m = l - c / 2;

    if (h < 60) {
        r = (c + m) * 255;
        g = (x + m) * 255;
        b = m * 255;
    } else if (h < 120) {
        r = (x + m) * 255;
        g = (c + m) * 255;
        b = m * 255;
    } else if (h < 180) {
        r = m * 255;
        g = (c + m) * 255;
        b = (x + m) * 255;
    } else if (h < 240) {
        r = m * 255;
        g = (x + m) * 255;
        b = (c + m) * 255;
    } else if (h < 300) {
        r = (x + m) * 255;
        g = m * 255;
        b = (c + m) * 255;
    } else {
        r = (c + m) * 255;
        g = m * 255;
        b = (x + m) * 255;
    }

    return Color(r,g,b);

}


static uint16_t hsv2hex(float h, float s, float v) {
    uint8_t r, g, b; 
    float f, p, q, t;

    if (s == 0) {
        // 灰色
        r = g = b = static_cast<uint8_t>(v * 255);
        return Color(r, g, b);
    }

    uint8_t h_int = static_cast<uint8_t>(h / 60);
    f = h / 60 - h_int;
    p = v * (1 - s)*255;
    q = v * (1 - f * s)*255;
    t = v * (1 - s * (1 - f))*255 ;

    switch (h_int) {
        case 0: r = v * 255; g = t; b = p; break;
        case 1: r = q; g = v * 255; b = p; break;
        case 2: r = p; g = v * 255; b = t; break;
        case 3: r = p; g = q; b = v * 255; break;
        case 4: r = t; g = p; b = v * 255; break;
        default: r = v * 255; g = p; b = q; break;
    }

    return HexColor(r, g, b);
}

#endif
