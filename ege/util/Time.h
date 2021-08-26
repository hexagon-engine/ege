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

class Time
{
public:
    enum class Unit
    {
        Ticks,
        Seconds
    };

    Time(MaxFloat value = 0, Unit unit = Unit::Seconds)
    : m_value(value)
    , m_unit(unit) {}

    MaxFloat getValue() const { return m_value; }
    Unit getUnit() const { return m_unit; }

    Time operator+(MaxFloat _2);
    Time& operator+=(MaxFloat _2);

    bool operator<(Time const& other) const
    {
        ASSERT(other.m_unit == m_unit);
        return m_value < other.m_value;
    }

    bool operator>(Time const& other) const
    {
        ASSERT(other.m_unit == m_unit);
        return m_value > other.m_value;
    }

    bool operator<=(Time const& other) const
    {
        ASSERT(other.m_unit == m_unit);
        return m_value <= other.m_value;
    }

    bool operator>=(Time const& other) const
    {
        ASSERT(other.m_unit == m_unit);
        return m_value >= other.m_value;
    }

private:
    MaxFloat m_value;
    Unit m_unit;
};

}

std::ostream& operator<<(std::ostream& _str, const EGE::Time& data);
