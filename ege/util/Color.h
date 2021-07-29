/*
*
*   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*
*      ,----  ,----  ,----
*      |      |      |
*      |----  | --,  |----
*      |      |   |  |
*      '----  '---'  '----
*
*     Framework Library for Hexagon
*
*    Copyright (c) Sppmacd 2020 - 2021
*
*    Permission is hereby granted, free of charge, to any person obtaining a copy
*    of this software and associated documentation files (the "Software"), to deal
*    in the Software without restriction, including without limitation the rights
*    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
*    copies of the Software, and to permit persons to whom the Software is
*    furnished to do so, subject to the following conditions:
*
*    The above copyright notice and this permission notice shall be included in all
*    copies or substantial portions of the Software.
*
*    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
*    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
*    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
*    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
*    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
*    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
*    SOFTWARE.
*
*   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*
*/

#pragma once

#include "Types.h"
#include <ege/main/Config.h>

namespace EGE
{

class ColorRGBA
{
public:
    ColorRGBA()
    : ColorRGBA({}, {}, {}) {}

    static ColorRGBA fromBytes(Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255) { return ColorRGBA(r, g, b, a); }
    static ColorRGBA fromFloats(float r, float g, float b, float a = 1) { return fromBytes(r * 255, g * 255, b * 255, a * 255); }
    
    // TODO: Move it out of line somehow
    static ColorRGBA fromHTML(String html, Uint8 alpha = 255);

    bool operator==(const ColorRGBA& other)
        { return other.r == r && other.g == g && other.b == b && other.a == a; }

    bool operator!=(const ColorRGBA& other)
        { return !(*this == other); }

    ColorRGBA operator+(const ColorRGBA& other)
        { return ColorRGBA(r + other.r, g + other.g, b + other.b, a + other.a); }

    ColorRGBA operator-(const ColorRGBA& other)
        { return ColorRGBA(r - other.r, g - other.g, b - other.b, a - other.a); }

    ColorRGBA operator*(double d)
        { return ColorRGBA(r*d, g*d, b*d, a*d); }

    ColorRGBA operator/(double d)
        { return ColorRGBA(r*d, g*d, b*d, a*d); }

    Uint8 r, g, b, a;

private:

    ColorRGBA(Uint8 _r, Uint8 _g, Uint8 _b, Uint8 _a = 255)
    : r(_r), g(_g), b(_b), a(_a) {}
};
// TODO: HSV, HSL, LCh, CMYK? ...

namespace Colors
{

#define ENUMERATE_COLOR(name, r, g, b, a) extern ColorRGBA name;

// TODO: Add all HTML colors
#define __ENUMERATE_COLORS \
    ENUMERATE_COLOR(transparent,   0,    0,    0,    0) \
    \
    ENUMERATE_COLOR(gray,          192,  192,  192,  255) \
    ENUMERATE_COLOR(red,           255,  0,    0,    255) \
    ENUMERATE_COLOR(green,         0,    255,  0,    255) \
    ENUMERATE_COLOR(blue,          0,    0,    255,  255) \
    ENUMERATE_COLOR(cyan,          0,    255,  255,  255) \
    ENUMERATE_COLOR(magenta,       255,  0,    255,  255) \
    ENUMERATE_COLOR(yellow,        255,  255,  0,    255) \
    ENUMERATE_COLOR(white,         255,  255,  255,  255) \
    \
    ENUMERATE_COLOR(black,         0,    0,    0,    255) \
    ENUMERATE_COLOR(darkRed,       128,  0,    0,    255) \
    ENUMERATE_COLOR(darkGreen,     0,    128,  0,    255) \
    ENUMERATE_COLOR(darkBlue,      0,    0,    128,  255) \
    ENUMERATE_COLOR(darkCyan,      0,    128,  128,  255) \
    ENUMERATE_COLOR(darkMagenta,   128,  0,    128,  255) \
    ENUMERATE_COLOR(darkYellow,    128,  128,  0,    255) \
    ENUMERATE_COLOR(darkGray,      128,  128,  128,  255) \

__ENUMERATE_COLORS

} // Colors

}

std::ostream& operator<<(std::ostream& _str, const EGE::ColorRGBA& data);
