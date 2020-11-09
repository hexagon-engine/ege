/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#include "Math.h"

#include <cmath>
#include <iostream>

#define DEGTORAD ((double)(180.0 / EGE::pi())) // 57.2957795...

namespace EGE
{

namespace Math
{

DegreeAngle rad2deg(RadianAngle angle)
{
    return angle * DEGTORAD;
}

RadianAngle deg2rad(DegreeAngle angle)
{
    return angle / DEGTORAD;
}

double map(double input, double iMin, double iMax, double oMin, double oMax)
{
    if(iMin > iMax) std::swap(iMin, iMax);
    if(oMin > oMax) std::swap(oMin, oMax);
    ASSERT(iMin - iMax != 0);
    return (input-iMin) / (iMax-iMin) * (oMax-oMin) + oMin;
}

} // Math

}
