/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#pragma once

#include <ege/util/Vector.h>
#include <vector>

namespace EGE
{

struct EquationResult
{
    enum Count
    {
        None,
        Discrete,
        Infinite
    };
    std::vector<double> results;
    Count resultCount;

    EquationResult(Count _count = None)
    : resultCount(_count) {}

    EquationResult(std::vector<double> _results)
    : results(_results), resultCount(Discrete) {}

    EquationResult(double _result)
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

class GenericEquation
{
public:
    virtual EquationResult solve() = 0;
};

class LinearEquation : public GenericEquation
{
public:
    LinearEquation(double a, double b)
    : m_a(a), m_b(b) {}

    LinearEquation(Vec2d pa, Vec2d pb);

    virtual EquationResult solve();

    double a() { return m_a; }
    double b() { return m_b; }

private:
    double m_a = 0.0;
    double m_b = 0.0;
};

class SquareEquation : public GenericEquation
{
public:
    SquareEquation(double a, double b, double c)
    : m_a(a), m_b(b), m_c(c) {}

    virtual EquationResult solve();

    double delta();

    double a() { return m_a; }
    double b() { return m_b; }
    double c() { return m_c; }

private:
    double m_a = 0.0;
    double m_b = 0.0;
    double m_c = 0.0;
};

namespace Equation
{

// Solve equation ax+b=0
EquationResult linear(double a, double b);

// Solve equation ax^2+bx+c=0
EquationResult square(double a, double b, double c);

} // Equation

}
