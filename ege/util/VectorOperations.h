/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

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
double lengthSquared(Vector2<T> vector)
{
    return (vector.x*vector.x + vector.y*vector.y + vector.z*vector.z)););
}

template<class T>
double length(Vector2<T> vector)
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

} // namespace EGE

//// 2D VECTORS

// Operations
template<class T>
EGE::Vector2<T> operator+(EGE::Vector2<T> _1, EGE::Vector2<T> _2)
{
    return EGE::Vector2<T>(_1.x + _2.x, _1.y + _2.y);
}

template<class T>
EGE::Vector2<T> operator-(EGE::Vector2<T> _1, EGE::Vector2<T> _2)
{
    return EGE::Vector2<T>(_1.x - _2.x, _1.y - _2.y);
}

template<class T>
EGE::Vector2<T> operator*(EGE::Vector2<T> _1, T _2)
{
    return EGE::Vector2<T>(_1.x * _2, _1.y * _2);
}

template<class T>
EGE::Vector2<T> operator/(EGE::Vector2<T> _1, T _2)
{
    ASSERT(_2 != 0);
    return EGE::Vector2<T>(_1.x / _2, _1.y / _2);
}

// Equal / Not equal
template<class T>
bool operator==(EGE::Vector2<T> _1, EGE::Vector2<T> _2)
{
    return _1.x == _2.x && _1.y == _2.y;
}

template<class T>
bool operator!=(EGE::Vector2<T> _1, EGE::Vector2<T> _2)
{
    return !(_1 == _2);
}

// Less / Not less
template<class T>
bool operator<(EGE::Vector2<T> _1, EGE::Vector2<T> _2)
{
    return EGE::VectorOperations::lengthSquared(_1) < EGE::VectorOperations::lengthSquared(_2);
}

template<class T>
bool operator>=(EGE::Vector2<T> _1, EGE::Vector2<T> _2)
{
    return !(_1 < _2);
}

// Greater / Not greater
template<class T>
bool operator>(EGE::Vector2<T> _1, EGE::Vector2<T> _2)
{
    return EGE::VectorOperations::lengthSquared(_1) > EGE::VectorOperations::lengthSquared(_2);
}

template<class T>
bool operator<=(EGE::Vector2<T> _1, EGE::Vector2<T> _2)
{
    return !(_1 > _2);
}

// Operations
template<class T>
EGE::Vector2<T>& operator+=(EGE::Vector2<T>& _1, EGE::Vector2<T> _2)
{
    return _1 = _1 + _2;
}

template<class T>
EGE::Vector2<T>& operator-=(EGE::Vector2<T>& _1, EGE::Vector2<T> _2)
{
    return _1 = _1 - _2;
}

template<class T>
EGE::Vector2<T>& operator*=(EGE::Vector2<T>& _1, T _2)
{
    return _1 = _1 * _2;
}

template<class T>
EGE::Vector2<T>& operator/=(EGE::Vector2<T>& _1, T _2)
{
    return _1 = _1 / _2;
}

//// 3D VECTORS

// Operations
template<class T>
EGE::Vector3<T> operator+(EGE::Vector3<T> _1, EGE::Vector3<T> _2)
{
    return EGE::Vector3<T>(_1.x + _2.x, _1.y + _2.y, _1.z + _2.z);
}

template<class T>
EGE::Vector3<T> operator-(EGE::Vector3<T> _1, EGE::Vector3<T> _2)
{
    return EGE::Vector3<T>(_1.x - _2.x, _1.y - _2.y, _1.z - _2.z);
}

template<class T>
EGE::Vector3<T> operator*(EGE::Vector3<T> _1, T _2)
{
    return EGE::Vector3<T>(_1.x * _2, _1.y * _2, _1.z * _2);
}

template<class T>
EGE::Vector3<T> operator/(EGE::Vector3<T> _1, T _2)
{
    ASSERT(_2 != 0);
    return EGE::Vector3<T>(_1.x / _2, _1.y / _2, _1.z / _2);
}

// Equal / Not equal
template<class T>
bool operator==(EGE::Vector3<T> _1, EGE::Vector3<T> _2)
{
    return _1.x == _2.x && _1.y == _2.y && _1.z == _2.z;
}

template<class T>
bool operator!=(EGE::Vector3<T> _1, EGE::Vector3<T> _2)
{
    return !(_1 == _2);
}

// Less / Not less
template<class T>
bool operator<(EGE::Vector3<T> _1, EGE::Vector3<T> _2)
{
    return EGE::VectorOperations::lengthSquared(_1) < EGE::VectorOperations::lengthSquared(_2);
}

template<class T>
bool operator>=(EGE::Vector3<T> _1, EGE::Vector3<T> _2)
{
    return !(_1 < _2);
}

// Greater / Not greater
template<class T>
bool operator>(EGE::Vector3<T> _1, EGE::Vector3<T> _2)
{
    return EGE::VectorOperations::lengthSquared(_1) > EGE::VectorOperations::lengthSquared(_2);
}

template<class T>
bool operator<=(EGE::Vector3<T> _1, EGE::Vector3<T> _2)
{
    return !(_1 > _2);
}

// Operations
template<class T>
EGE::Vector3<T>& operator+=(EGE::Vector3<T>& _1, EGE::Vector3<T> _2)
{
    return _1 = _1 + _2;
}

template<class T>
EGE::Vector3<T>& operator-=(EGE::Vector3<T>& _1, EGE::Vector3<T> _2)
{
    return _1 = _1 - _2;
}

template<class T>
EGE::Vector3<T>& operator*=(EGE::Vector3<T>& _1, T _2)
{
    return _1 = _1 * _2;
}

template<class T>
EGE::Vector3<T>& operator/=(EGE::Vector3<T>& _1, T _2)
{
    return _1 = _1 / _2;
}
