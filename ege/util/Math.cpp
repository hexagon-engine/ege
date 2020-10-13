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

} // Math

}
