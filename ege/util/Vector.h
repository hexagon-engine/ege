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

// 2D Vectors
template<class T>
class Vector2
{
public:
    Vector2()
    : Vector2({},{}) {}

    Vector2(T _x, T _y)
    : x(_x), y(_y) {}

    template<class V>
    Vector2(const Vector2<V>& another)
    : Vector2(another.x, another.y) {}

    T x,y;
};

typedef Vector2<Float> Vec2f;
typedef Vector2<Double> Vec2d;
typedef Vector2<Int32> Vec2i;
typedef Vector2<Uint32> Vec2u;
typedef Vector2<Size> Vec2s;

// 3D Vectors
template<class T>
class Vector3
{
public:
    Vector3()
    : Vector3({},{},{}) {}

    Vector3(T _x, T _y)
    : x(_x), y(_y), z({}) {}

    Vector3(const Vector2<T>& other)
    : x(other.x), y(other.y), z({}) {}

    Vector3(T _x, T _y, T _z)
    : x(_x), y(_y), z(_z) {}

    template<class V>
    Vector3(const Vector3<V>& another)
    : Vector3(another.x, another.y, another.z) {}

    T x,y,z;
};

typedef Vector3<Float> Vec3f;
typedef Vector3<Double> Vec3d;
typedef Vector3<Int32> Vec3i;
typedef Vector3<Uint32> Vec3u;

// Polar and Spherical Vectors
template<class T>
class PolarVector2
{
public:
    PolarVector2()
    : PolarVector2({},{}) {}

    PolarVector2(T _a, T _l)
    : angle(_a), length(_l) {}

    template<class V>
    PolarVector2(const PolarVector2<V>& another)
    : PolarVector2(another.angle, another.length) {}

    T angle,length;
};

typedef PolarVector2<Float> PolVec2f;
typedef PolarVector2<Double> PolVec2d;
typedef PolarVector2<Int32> PolVec2i;
typedef PolarVector2<Uint32> PolVec2u;

}

template<class T>
std::ostream& operator<<(std::ostream& _str, const EGE::Vector2<T>& data)
{
    return _str << "(" << data.x << "," << data.y << ")";
}

template<class T>
std::ostream& operator<<(std::ostream& _str, const EGE::Vector3<T>& data)
{
    return _str << "(" << data.x << "," << data.y << data.z << ")";
}

template<class T>
std::ostream& operator<<(std::ostream& _str, const EGE::PolarVector2<T>& data)
{
    return _str << "[" << data.length << "*" << data.angle << "deg]";
}

#define VECTOR_H

#include "VectorOperations.h"

#define VECTOR_H_END
