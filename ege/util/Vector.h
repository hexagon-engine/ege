/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#pragma once

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

typedef Vector2<float> Vec2f;
typedef Vector2<double> Vec2d;
typedef Vector2<int> Vec2i;
typedef Vector2<unsigned> Vec2u;

// 3D Vectors
template<class T>
class Vector3
{
public:
    Vector3()
    : Vector3({},{},{}) {}

    Vector3(T _x, T _y, T _z)
    : x(_x), y(_y), z(_z) {}

    template<class V>
    Vector3(const Vector3<V>& another)
    : Vector3(another.x, another.y, another.z) {}

    T x,y,z;
};

typedef Vector3<float> Vec3f;
typedef Vector3<double> Vec3d;
typedef Vector3<int> Vec3i;
typedef Vector3<unsigned> Vec3u;

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
    PolarVector2(const Vector3<V>& another)
    : PolarVector2(another.angle, another.length) {}

    T angle,length;
};

typedef PolarVector2<float> PolVec2f;
typedef PolarVector2<double> PolVec2d;
typedef PolarVector2<int> PolVec2i;
typedef PolarVector2<unsigned> PolVec2u;

}

#define VECTOR_H

#include "VectorOperations.h"

#define VECTOR_H_END
