/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#pragma once

namespace EGE
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

}
