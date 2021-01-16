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
#include "Vector.h"

#include <ege/main/Config.h>

namespace EGE
{

template<class T>
class Rect
{
public:
    typedef T Type;

    Rect() = default;

    Rect(T left, T top, T width, T height)
    : position(left, top), size(width, height) { ASSERT(width >= 0 && height >= 0); }

    Rect(Vector2<T> _position, Vector2<T> _size)
    : position(_position), size(_size) { ASSERT(size.x >= 0 && size.y >= 0); }

    Vector2<T> position;
    Vector2<T> size;

    T area() const { return size.x * size.y; }
    T perimeter() const { return 2 * (size.x + size.y); }
    bool empty() const { return size == Vector2<T>(); }

    bool contains(Vector2<T> pos) const
        { return pos.x >= position.x && pos.y >= position.y && pos.x <= position.x + size.x && pos.y <= position.y + size.y; }

    Rect<T> intersection(Rect<T> other) const
    {
        T max_x = std::max(position.x, other.position.x);
        T max_y = std::max(position.y, other.position.y);

        T min_x = std::min(position.x + size.x, other.position.x + other.size.x);
        T min_y = std::min(position.y + size.y, other.position.y + other.size.y);

        return max_x < min_x && max_y < min_y ? Rect<T>({max_x, max_y}, {min_x - max_x, min_y - max_y}) : Rect<T>();
    }

    Rect<T> operator+(const Rect<T>& other) const
        { return Rect<T>(other.position + position, other.size + size); }

    Rect<T> operator-(const Rect<T>& other) const
        { return Rect<T>(other.position - position, other.size - size); }

    Rect<T> operator*(T d) const
        { return Rect<T>(position * d, size * d); }

    Rect<T> operator/(T d) const
        { return Rect<T>(position / d, size / d); }

    Rect<T> operator*(const Rect<T>& other) const
        { return intersection(other); }

    bool operator==(const Rect<T>& other) const
        { return position == other.position && size == other.size; }

    bool operator!=(const Rect<T>& other) const
        { return !(*this == other); }
};

typedef Rect<Float> RectF;
typedef Rect<Double> RectD;
typedef Rect<Int32> RectI;
typedef Rect<Uint32> RectU;
typedef Rect<Size> RectS;

}
