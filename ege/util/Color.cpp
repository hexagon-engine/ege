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

#include "Color.h"

#include <ege/main/Config.h>

namespace EGE
{

namespace Colors
{

#undef ENUMERATE_COLOR
#define ENUMERATE_COLOR(name, r, g, b, a) ColorRGBA name(r, g, b, a);

__ENUMERATE_COLORS

}

ColorRGBA ColorRGBA::fromHTML(String html, Uint8 alpha)
{
    // TODO: Support short format
    String r = html.substr(1, 2);
    String g = html.substr(3, 2);
    String b = html.substr(5, 2);
    try
    {
        Uint8 ri = std::stoi(r, nullptr, 16);
        Uint8 gi = std::stoi(g, nullptr, 16);
        Uint8 bi = std::stoi(b, nullptr, 16);
        return ColorRGBA::fromBytes(ri, gi, bi, alpha);
    }
    catch(...)
    {
        CRASH_WITH_MESSAGE("Invalid HTML color");
    }
}

}

std::ostream& operator<<(std::ostream& _str, const EGE::ColorRGBA& data)
{
    return _str << "ColorRGBA(" << data.r << ',' << data.g << ',' << data.b << ',' << data.a << ')';
}

