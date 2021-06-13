/*
*
*   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*
*      ,----  ,----  ,----
*      |      |      |
*      |----  | --,  |----
*      |      |   |  |
*      '----  '---'  '----
*
*     Framework Library for Hexagon
*
*    Copyright (c) Sppmacd 2020 - 2021
*
*    Permission is hereby granted, free of charge, to any person obtaining a copy
*    of this software and associated documentation files (the "Software"), to deal
*    in the Software without restriction, including without limitation the rights
*    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
*    copies of the Software, and to permit persons to whom the Software is
*    furnished to do so, subject to the following conditions:
*
*    The above copyright notice and this permission notice shall be included in all
*    copies or substantial portions of the Software.
*
*    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
*    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
*    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
*    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
*    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
*    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
*    SOFTWARE.
*
*   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*
*/

#pragma once

#include <vector>
#include <ege/util/Vector.h>

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

    bool operator==(Count _count) const
    {
        return resultCount == _count;
    }

    // slow
    bool operator==(std::vector<double> _results) const
    {
        return results == _results;
    }

    bool operator==(double _result) const
    {
        return results.size() == 1 && results.back() == _result;
    }
};

class GenericEquation
{
public:
    virtual EquationResult solve() const = 0;
};

class LinearEquation : public GenericEquation
{
public:
    LinearEquation(double a, double b)
    : m_a(a), m_b(b) {}

    LinearEquation(Vec2d pa, Vec2d pb);

    virtual EquationResult solve() const;

    double a() const { return m_a; }
    double b() const { return m_b; }

private:
    double m_a = 0.0;
    double m_b = 0.0;
};

class SquareEquation : public GenericEquation
{
public:
    SquareEquation(double a, double b, double c)
    : m_a(a), m_b(b), m_c(c) {}

    virtual EquationResult solve() const;

    double delta() const;

    double a() const { return m_a; }
    double b() const { return m_b; }
    double c() const { return m_c; }

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
