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

namespace Equation
{

Result linear(double a, double b)
{
    if(a == 0)
    {
        std::cerr << "linear: static equation" << std::endl;
        // Static equation: b = 0
        if(b == 0)
            return { Result::Infinite };
        else
            return {};
    }
    else
    {
        std::cerr << "linear: 1 solution" << std::endl;
        return -b / a;
    }
}

Result square(double a, double b, double c)
{
    if(a == 0)
    {
        std::cerr << "square: linear equation" << std::endl;
        // Linear equation: bx+c
        return linear(b, c);
    }

    double delta = b*b - 4*a*c;
    std::cerr << "square: delta=" << delta << std::endl;

    if(delta < 0)
        return {};
    else if(delta == 0) // -b/2a
        return -b / (2 * a);
    else
    {
        std::cerr << "square: 2 solutions" << std::endl;
        double sqrt_delta = std::sqrt(delta);
        return {{ (-b - sqrt_delta) / (2 * a), (-b + sqrt_delta) / (2 * a) }};
    }
}

} // Equation

} // Math

}
