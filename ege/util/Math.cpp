/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#include "Math.h"

namespace EGE
{

#define DEGTORAD ((double)(180.0 / EGE::pi()))

DegreeAngle rad2deg(RadianAngle angle)
{
    return angle * DEGTORAD;
}

RadianAngle deg2rad(DegreeAngle angle)
{
    return angle / DEGTORAD;
}

}
