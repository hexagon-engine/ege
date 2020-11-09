/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#pragma once

#include <vector>

#include <ege/main/Config.h>

namespace EGE
{

namespace Math
{

// Angle
typedef double DegreeAngle;
typedef double RadianAngle;
typedef DegreeAngle Angle;

// Radian <-> Degrees
DegreeAngle rad2deg(RadianAngle angle);
RadianAngle deg2rad(DegreeAngle angle);

// Constants
constexpr double pi()
{
    return 3.141592653589793;
}

constexpr double e()
{
    return 2.718281828459045;
}

double map(double input, double iMin, double iMax, double oMin, double oMax);

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
