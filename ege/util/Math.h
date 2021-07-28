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

#include <vector>

#include <ege/main/Config.h>

namespace EGE
{

namespace Math
{

// Constants
template<class T = double>
inline constexpr T pi() { return 3.141592653589793; }

template<class T = double>
inline constexpr T e() { return 2.718281828459045; }

template<class T = double>
inline constexpr T deg_to_rad() { return 180.0 / pi<T>(); } // 57.2957795...

// Angle
typedef double DegreeAngle;
typedef double RadianAngle;
typedef DegreeAngle Angle;

// Radian <-> Degrees
template<class T>
T rad2deg(T angle)
{
    return angle * deg_to_rad<T>();
}

template<class T>
T deg2rad(T angle)
{
    return angle / deg_to_rad<T>();
}

template<class T>
T map(T input, T iMin, T iMax, T oMin, T oMax)
{
    if(iMin > iMax) std::swap(iMin, iMax);
    if(oMin > oMax) std::swap(oMin, oMax);
    ASSERT(iMin - iMax != 0);
    return (input-iMin) / (iMax-iMin) * (oMax-oMin) + oMin;
}

} // Math

using Math::DegreeAngle;
using Math::RadianAngle;
using Math::Angle;
using Math::rad2deg;
using Math::deg2rad;
using Math::pi;
using Math::e;
using Math::map;

}
