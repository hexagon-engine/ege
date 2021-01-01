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

// It should be included only from Vector.h!
#if !defined(VECTOR_H) || defined(VECTOR_H_END)
#error "VectorOperations should not be included directly"
#endif

#include "Math.h"

#include <cmath>
#include <ege/main/Config.h>
#include <ege/util/PointerUtils.h>

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

} // namespace VectorOperations

//// 2D VECTORS

// Operations
template<class T>
EGE::Vector2<T> operator+(const EGE::Vector2<T> _1, const EGE::Vector2<T> _2)
{
    return EGE::Vector2<T>(_1.x + _2.x, _1.y + _2.y);
}

template<class T>
EGE::Vector2<T> operator-(const EGE::Vector2<T> _1, const EGE::Vector2<T> _2)
{
    return EGE::Vector2<T>(_1.x - _2.x, _1.y - _2.y);
}

template<class T>
EGE::Vector2<T> operator*(const EGE::Vector2<T> _1, const T _2)
{
    return EGE::Vector2<T>(_1.x * _2, _1.y * _2);
}

template<class T>
EGE::Vector2<T> operator/(const EGE::Vector2<T> _1, const T _2)
{
    ASSERT(_2 != 0);
    return EGE::Vector2<T>(_1.x / _2, _1.y / _2);
}

// Equal / Not equal
template<class T>
bool operator==(const EGE::Vector2<T> _1, const EGE::Vector2<T> _2)
{
    return _1.x == _2.x && _1.y == _2.y;
}

template<class T>
bool operator!=(const EGE::Vector2<T> _1, const EGE::Vector2<T> _2)
{
    return !(_1 == _2);
}

// Less / Not less
template<class T>
bool operator<(const EGE::Vector2<T> _1, const EGE::Vector2<T> _2)
{
    return _1.x > _2.x || (_1.x == _2.x && _1.y > _2.y);
}

template<class T>
bool operator>=(const EGE::Vector2<T> _1, const EGE::Vector2<T> _2)
{
    return !(_1 < _2);
}

// Greater / Not greater
template<class T>
bool operator>(const EGE::Vector2<T> _1, const EGE::Vector2<T> _2)
{
    return _1.x < _2.x || (_1.x == _2.x && _1.y < _2.y);
}

template<class T>
bool operator<=(const EGE::Vector2<T> _1, const EGE::Vector2<T> _2)
{
    return !(_1 > _2);
}

// Operations
template<class T>
EGE::Vector2<T>& operator+=(EGE::Vector2<T>& _1, const EGE::Vector2<T> _2)
{
    return _1 = _1 + _2;
}

template<class T>
EGE::Vector2<T>& operator-=(EGE::Vector2<T>& _1, const EGE::Vector2<T> _2)
{
    return _1 = _1 - _2;
}

template<class T>
EGE::Vector2<T>& operator*=(EGE::Vector2<T>& _1, const T _2)
{
    return _1 = _1 * _2;
}

template<class T>
EGE::Vector2<T>& operator/=(EGE::Vector2<T>& _1, const T _2)
{
    return _1 = _1 / _2;
}

//// 3D VECTORS

// Operations
template<class T>
EGE::Vector3<T> operator+(const EGE::Vector3<T> _1, const EGE::Vector3<T> _2)
{
    return EGE::Vector3<T>(_1.x + _2.x, _1.y + _2.y, _1.z + _2.z);
}

template<class T>
EGE::Vector3<T> operator-(const EGE::Vector3<T> _1, const EGE::Vector3<T> _2)
{
    return EGE::Vector3<T>(_1.x - _2.x, _1.y - _2.y, _1.z - _2.z);
}

template<class T>
EGE::Vector3<T> operator*(const EGE::Vector3<T> _1, const T _2)
{
    return EGE::Vector3<T>(_1.x * _2, _1.y * _2, _1.z * _2);
}

template<class T>
EGE::Vector3<T> operator/(const EGE::Vector3<T> _1, const T _2)
{
    ASSERT(_2 != 0);
    return EGE::Vector3<T>(_1.x / _2, _1.y / _2, _1.z / _2);
}

// Equal / Not equal
template<class T>
bool operator==(const EGE::Vector3<T> _1, const EGE::Vector3<T> _2)
{
    return _1.x == _2.x && _1.y == _2.y && _1.z == _2.z;
}

template<class T>
bool operator!=(const EGE::Vector3<T> _1, const EGE::Vector3<T> _2)
{
    return !(_1 == _2);
}

// Less / Not less
template<class T>
bool operator<(const EGE::Vector3<T> _1, const EGE::Vector3<T> _2)
{
    return EGE::VectorOperations::lengthSquared(_1) < EGE::VectorOperations::lengthSquared(_2);
}

template<class T>
bool operator>=(const EGE::Vector3<T> _1, const EGE::Vector3<T> _2)
{
    return !(_1 < _2);
}

// Greater / Not greater
template<class T>
bool operator>(const EGE::Vector3<T> _1, const EGE::Vector3<T> _2)
{
    return EGE::VectorOperations::lengthSquared(_1) > EGE::VectorOperations::lengthSquared(_2);
}

template<class T>
bool operator<=(const EGE::Vector3<T> _1, const EGE::Vector3<T> _2)
{
    return !(_1 > _2);
}

// Operations
template<class T>
EGE::Vector3<T>& operator+=(EGE::Vector3<T>& _1, const EGE::Vector3<T> _2)
{
    return _1 = _1 + _2;
}

template<class T>
EGE::Vector3<T>& operator-=(EGE::Vector3<T>& _1, const EGE::Vector3<T> _2)
{
    return _1 = _1 - _2;
}

template<class T>
EGE::Vector3<T>& operator*=(EGE::Vector3<T>& _1, const T _2)
{
    return _1 = _1 * _2;
}

template<class T>
EGE::Vector3<T>& operator/=(EGE::Vector3<T>& _1, const T _2)
{
    return _1 = _1 / _2;
}

} // namespace EGE
