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

#include "EquationSystem.h"

#include "Equation.h"

#include <iomanip>
#include <sstream>

namespace EGE
{

void SquareMatrix::init(const std::vector<std::vector<double>>& values)
{
    // Check if matrix created from values will be actually square.
    ASSERT(!values.empty());

    size_t lastSize = values.front().size();

    // it must be square!
    for(const std::vector<double>& row : values)
    {
        if(row.size() != values.size() || row.size() != lastSize)
        {
            std::cerr << "Square matrix must be square and must have equally-sized rows!" << std::endl;
            CRASH();
        }
    }

    m_values = values;
}

void SquareMatrix::setColumn(size_t column, const std::vector<double>& values)
{
    size_t _size = size();
    ASSERT(column < _size);
    ASSERT(values.size() == _size);

    for(size_t s = 0; s < _size; s++)
    {
        value(column, s) = values[s];
    }
}

double SquareMatrix::det() const
{
    if(size() == 1)
    {
        return value(0, 0);
    }
    if(size() == 2)
    {
        return value(0, 0) * value(1, 1)
             - value(1, 0) * value(0, 1);
    }
    else if(size() == 3)
    {
        double v1 = value(0, 0) * value(1, 1) * value(2, 2);
        double v2 = value(1, 0) * value(2, 1) * value(0, 2);
        double v3 = value(2, 0) * value(0, 1) * value(1, 2);
        double v4 = value(0, 2) * value(1, 1) * value(2, 0);
        double v5 = value(1, 2) * value(2, 1) * value(0, 0);
        double v6 = value(2, 2) * value(0, 1) * value(1, 0);
        return v1 +v2 +v3 -v4 -v5 -v6;
    }
    else
    {
        // We don't support them yet.
        CRASH();
    }
}

std::string SquareMatrix::toString() const
{
    std::ostringstream oss;
    for(const std::vector<double>& vals: m_values)
    {
        oss << "\u2502 "; // |
        for(double val: vals)
        {
            oss << std::setw(10) << val << " ";
        }
        oss << " \u2502" << std::endl; // |
    }
    return oss.str();
}

EquationResult EquationSystemResult::resultFor(size_t variable) const
{
    ASSERT(variable < m_results.size());
    return m_results[variable];
}

void EquationSystemResult::setResult(size_t variable, const EquationResult& result)
{
    ASSERT(variable < m_results.size());
    m_results[variable] = result;
}

EquationSystemResult LinearEquationSystem::solve() const
{
    if(solutions() == 1)
    {
        // Simple linear equation.
        // ax = b ==> ax - b = 0
        // We use LinearEquation.solve() for that simple equation
        // to handle infinite or null solutions.
        EquationSystemResult result(1);
        result.setResult(0, LinearEquation(m_as.value(0, 0), -m_bs[0]).solve());
        return result;
    }
    else
    {
        // Equation system with N solutions / N equations.

        // Compute determinant.
        double det = m_as.det();

        EquationSystemResult result(solutions());
        for(size_t s = 0; s < m_as.size(); s++)
        {
            // Compute determinant for each X.
            SquareMatrix mat = m_as;
            mat.setColumn(s, m_bs);
            double xdet = mat.det();

            // Compute result.
            // xn = det An / det A
            if(det == 0)
            {
                if(xdet == 0)
                    result.setResult(s, EquationResult::Infinite);
                else
                {
                    // We don't need to calculate anything; we already now
                    // that the equation system has no results.
                    for(size_t s2 = 0; s2 < m_as.size(); s2++)
                        result.setResult(s2, EquationResult::None);
                    return result;
                }
            }
            else
                result.setResult(s, xdet / det);
        }
        return result;
    }
}

namespace EquationSystem
{

EquationSystemResult linear(const SquareMatrix& as, const std::vector<double>& bs)
{
    return LinearEquationSystem(as, bs).solve();
}

} // EquationSystem

}
