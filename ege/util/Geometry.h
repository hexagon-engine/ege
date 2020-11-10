/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#include "Vector.h"

namespace EGE
{

namespace Geometry
{

// Vector and Line
template<class T>
T distanceToLine(T A, T B, T C, T x0, T y0)
{
    return (double)std::abs(A*x0+B*y0+C) / std::sqrt(A*A + B*B);
}

enum class LineRelation
{
    NoRelation,
    Equal,
    Parallel,
    Perpendicular
};

// Lines of form Ax+By+C=0
template<class T>
LineRelation lineRelation(double A1, double B1, double C1, double A2, double B2, double C2)
{
    (void)A1;
    (void)B1;
    (void)C1;
    (void)A2;
    (void)B2;
    (void)C2;
    // TODO
    return LineRelation::NoRelation;
}

} // Geometry

}
