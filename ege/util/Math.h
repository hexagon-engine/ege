/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#pragma once

#include <vector>

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

namespace Equation
{

struct Result
{
    enum Count
    {
        None,
        Discrete,
        Infinite
    };
    std::vector<double> results;
    Count resultCount;

    Result(Count _count = None)
    : resultCount(_count) {}

    Result(std::vector<double> _results)
    : results(_results), resultCount(Discrete) {}

    Result(double _result)
    : results({_result}), resultCount(Discrete) {}

    bool operator==(Count _count)
    {
        return resultCount == _count;
    }

    // slow
    bool operator==(std::vector<double> _results)
    {
        return results == _results;
    }

    bool operator==(double _result)
    {
        return results.size() == 1 && results.back() == _result;
    }
};

// Solve equation ax+b=0
Result linear(double a, double b);

// Solve equation ax^2+bx+c=0
Result square(double a, double b, double c);

}

} // Math

using Math::DegreeAngle;
using Math::RadianAngle;
using Math::Angle;
using Math::rad2deg;
using Math::deg2rad;
using Math::pi;
using Math::e;

}
