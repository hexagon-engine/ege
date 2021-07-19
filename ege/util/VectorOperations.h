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

#include "Math.h"

#include <cmath>
#include <ege/main/Config.h>
#include "PointerUtils.h"
#include "EquationSystem.h"
#include "Vector.h"

namespace EGE
{

namespace VectorOperations
{

// Vector2
template<class T>
DegreeAngle angleTo(Vector2<T> begin, Vector2<T> end)
{
    T dx = end.x - begin.x;
    T dy = end.y - begin.y;
    return rad2deg(std::atan2(dx, dy));
}

template<class T>
double distanceTo(Vector2<T> begin, Vector2<T> end)
{
    T dx = end.x - begin.x;
    T dy = end.y - begin.y;
    return std::sqrt((double)(dx*dx+dy*dy));
}

template<class T>
double lengthSquared(Vector2<T> vector)
{
    return (vector.x*vector.x+vector.y*vector.y);
}

template<class T>
double length(Vector2<T> vector)
{
    return std::sqrt(lengthSquared(vector));
}

// Vector3
template<class T>
double distanceTo(Vector3<T> begin, Vector3<T> end)
{
    T dx = end.x - begin.x;
    T dy = end.y - begin.y;
    T dz = end.z - begin.z;
    return std::sqrt((double)(dx*dx+dy*dy+dz*dz));
}

template<class T>
double lengthSquared(Vector3<T> vector)
{
    return (vector.x*vector.x + vector.y*vector.y + vector.z*vector.z);
}

template<class T>
double length(Vector3<T> vector)
{
    return std::sqrt(lengthSquared(vector));
}

// 2D Polar <-> Cartesian
template<class T>
Vector2<T> fromPolar(PolarVector2<T> polar)
{
    return Vector2<T>(polar.length * std::sin(deg2rad(polar.angle)), polar.length * std::cos(deg2rad(polar.angle)));
}

template<class T>
PolarVector2<T> toPolar(Vector2<T> cartesian)
{
    return PolarVector2<T>(angleTo(Vector2<T>(), cartesian), distanceTo(Vector2<T>(), cartesian));
}

// 2D Transforms
template<class T>
Vector2<T> scale(Vector2<T> in, Vector2<T> factors, Vector2<T> center = {})
{
    Vector2<T> tmp = in - center;
    tmp.x *= factors.x;
    tmp.y *= factors.y;
    return tmp + center;
}

template<class T>
Vector2<T> scale(Vector2<T> in, T factor, Vector2<T> center = {})
{
    return scale(in, {factor, factor}, center);
}

template<class T>
Vector2<T> rotate(Vector2<T> in, double angle, Vector2<T> center = {})
{
    Vector2<T> tmp = in - center;
    Vector2<T> out;
    out.x = std::cos(Math::deg2rad(-angle)) * tmp.x - std::sin(Math::deg2rad(-angle)) * tmp.y;
    out.y = std::sin(Math::deg2rad(-angle)) * tmp.x + std::cos(Math::deg2rad(-angle)) * tmp.y;
    return out + center;
}

template<class T>
Vector3<T> transform(const Vector3<T>& in, const SquareMatrix& matrix)
{
    // FIXME: Support matrices with size other than 3
    ASSERT(matrix.size() == 3);
    Vector3<T> out;
    out.x = in.x * matrix.value(0, 0) + in.y * matrix.value(0, 1) + in.z * matrix.value(0, 2);
    out.y = in.x * matrix.value(1, 0) + in.y * matrix.value(1, 1) + in.z * matrix.value(1, 2);
    out.z = in.x * matrix.value(2, 0) + in.y * matrix.value(2, 1) + in.z * matrix.value(2, 2);
    return out;
}

// TODO: Tests
template<class T>
Vector3<T> rotateYawPitchRoll(Vector3<T> in, double yaw, double pitch, double roll, Vector3<T> center = {})
{
    Vector3<T> tmp = in - center;
    double yawR = Math::deg2rad(yaw);
    double pitchR = Math::deg2rad(pitch);
    double rollR = Math::deg2rad(roll);
    auto yawMatrix = SquareMatrix({{cos(yawR), -sin(yawR), 0}, {sin(yawR), cos(yawR), 0}, {0, 0, 1}});
    auto pitchMatrix = SquareMatrix({{cos(pitchR), 0, sin(pitchR)}, {0, 1, 0}, {-sin(pitchR), 0, cos(pitchR)}});
    auto rollMatrix = SquareMatrix({{1, 0, 0}, {0, cos(rollR), -sin(rollR)}, {0, sin(rollR), cos(rollR)}});
    Vector3<T> out = transform(tmp, yawMatrix * pitchMatrix * rollMatrix);
    return out + center;
}

} // namespace VectorOperations

// Less / Not less
template<class T>
bool operator<(const EGE::Vector3<T> _1, const EGE::Vector3<T> _2)
{
    return EGE::VectorOperations::lengthSquared(_1) < EGE::VectorOperations::lengthSquared(_2);
}

// Greater / Not greater
template<class T>
bool operator>(const EGE::Vector3<T> _1, const EGE::Vector3<T> _2)
{
    return EGE::VectorOperations::lengthSquared(_1) > EGE::VectorOperations::lengthSquared(_2);
}

} // namespace EGE
