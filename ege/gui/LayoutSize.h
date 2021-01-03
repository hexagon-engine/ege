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

#include <ege/util/Types.h>
#include <ege/util/Vector.h>
#include <sstream>

#define EGE_LAYOUT_PIXELS "px"
#define EGE_LAYOUT_FILL "N"
#define EGE_LAYOUT_PERCENT "%"
#define EGE_LAYOUT_AUTO "a"

namespace EGE
{

template<class T>
class LayoutSize
{
public:
    LayoutSize()
    : m_value(T()), m_unit(EGE_LAYOUT_FILL) {}

    // Units: px, % (of parent), N (fill), a (total size of children)
    LayoutSize(T val, String unit = EGE_LAYOUT_PIXELS)
    : m_value(val), m_unit(unit) {}

    explicit LayoutSize(String val)
    {
        std::istringstream iss;
        iss.str(val);
        iss >> m_value >> m_unit;
    }

    T value() const { return m_value; }
    void setValue(T val) { m_value = val; }

    String unit() const { return m_unit; }
    void setUnit(String unit) { m_unit = unit; }

    // Can value be calculated without calculating children's
    // layout (px - fixed size in pixels or A - total children size)
    bool isFixedUnit() { return m_unit == EGE_LAYOUT_PIXELS || m_unit == EGE_LAYOUT_AUTO; }

private:
    T m_value {};
    String m_unit = "N";
};

template<class T>
class LayoutVector2
{
public:
    LayoutVector2(String _x, String _y)
    : x(LayoutSize<T>(_x)), y(LayoutSize<T>(_y)) {}

    LayoutVector2(LayoutSize<T> _x = {}, LayoutSize<T> _y = {})
    : x(_x), y(_y) {}

    LayoutVector2(Vector2<T> vec)
    : x(vec.x), y(vec.y) {}

    LayoutSize<T> x, y;
};

typedef LayoutSize<int>    LayoutSizeI;
typedef LayoutSize<double> LayoutSizeD;

typedef LayoutVector2<int>    LVec2i;
typedef LayoutVector2<double> LVec2d;

template<class T>
std::ostream& operator<<(std::ostream& _stream, LayoutSize<T> _val)
{
    return _stream << _val.value() << _val.unit();
}

}
