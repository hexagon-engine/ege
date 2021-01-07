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

namespace EGE
{

class ColorRGBA
{
public:
    ColorRGBA(float _r, float _g, float _b, float _a = 1)
    : r(_r), g(_g), b(_b), a(_a) {}

    ColorRGBA()
    : ColorRGBA(0.f, 0.f, 0.f) {}

    static ColorRGBA fromBytes(Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255)
        { return ColorRGBA(r / 256.0, g / 256.0, b / 256.0, a / 256.0); }

    // TODO: Color from hex (HTML/CSS) format

    bool operator==(const ColorRGBA& other)
        { return other.r == r && other.g == g && other.b == b && other.a == a; }

    bool operator!=(const ColorRGBA& other)
        { return !(*this == other); }

    float r, g, b, a;
};

// TODO: HSV, HSL, LCh, CMYK? ...

namespace Colors
{

#define ENUMERATE_COLOR(name, r, g, b, a) extern ColorRGBA name;

// TODO: Add all HTML colors
#define __ENUMERATE_COLORS \
    ENUMERATE_COLOR(transparent,   0,   0,   0,   0) \
    ENUMERATE_COLOR(black,         0,   0,   0,   1) \
    ENUMERATE_COLOR(white,         1,   1,   1,   1) \
    ENUMERATE_COLOR(red,           1,   0,   0,   1) \
    ENUMERATE_COLOR(green,         0,   1,   0,   1) \
    ENUMERATE_COLOR(blue,          0,   0,   1,   1) \
    ENUMERATE_COLOR(cyan,          0,   1,   1,   1) \
    ENUMERATE_COLOR(magenta,       1,   0,   1,   1) \
    ENUMERATE_COLOR(yellow,        1,   1,   0,   1)

__ENUMERATE_COLORS

} // Colors

}
