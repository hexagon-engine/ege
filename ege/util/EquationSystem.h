/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#pragma once

#include "Equation.h"

#include <vector>

namespace EGE
{

class SquareMatrix
{
public:
    // { {a,b}, {c,d} }
    //
    // is transformed to
    //
    // | a b |
    // | c d |
    SquareMatrix(const std::vector<std::vector<double>>& values)
    {
        init(values);
    }

    SquareMatrix(const std::initializer_list<std::initializer_list<double>>& values)
    {
        std::vector<std::vector<double>> vals;
        for(const std::initializer_list<double> initlist : values)
        {
            vals.push_back(std::vector(initlist.begin(), initlist.end()));
        }
        init(vals);
    }

    inline double& value(size_t x, size_t y)
    {
        ASSERT(y < m_values.size());
        ASSERT(x < m_values[y].size());
        return m_values[y][x];
    }

    inline const double& value(size_t x, size_t y) const
    {
        ASSERT(y < m_values.size());
        ASSERT(x < m_values[y].size());
        return m_values[y][x];
    }

    inline size_t size() const
    {
        return m_values.size();
    }

    void setColumn(size_t column, const std::vector<double>& values);
    double det() const;

    std::string toString() const;

private:
    void init(const std::vector<std::vector<double>>& values);

    std::vector<std::vector<double>> m_values;
};

class EquationSystemResult
{
public:
    explicit EquationSystemResult(size_t size)
    {
        m_results.resize(size);
    }

    EquationResult resultFor(size_t variable) const;
    void setResult(size_t variable, const EquationResult& result);

private:
    std::vector<EquationResult> m_results;
};

class GenericEquationSystem
{
public:
    virtual EquationSystemResult solve() const = 0;
};

class LinearEquationSystem : GenericEquationSystem
{
public:
    // Equation system of the form
    //
    // { a_11*x_0 + a_12*x_1 + a_13*x_2 + ... + a1n*dx_n = b1
    // { a_21*x_0 + a_22*x_1 + a_23*x_2 + ... + a2n*x_n = b2
    // { a_31*x_0 + a_32*x_1 + a_33*x_2 + ... + a3n*x_n = b3
    // { ...
    // { a_n1*x_0 + a_n2*x_1 + a_n3*x_2 + ... + ann*x_n = bn
    //
    // Solution: EquationSystemResult{x_1, x_2, x_3, ...}
    LinearEquationSystem(const SquareMatrix& as, const std::vector<double>& bs)
    : m_as(as), m_bs(bs) { ASSERT(m_as.size() == m_bs.size()); ASSERT(m_as.size() != 0); }

    virtual EquationSystemResult solve() const;

    // They should be the same.
    inline size_t solutions() const { return m_as.size(); }

private:
    SquareMatrix m_as;
    std::vector<double> m_bs;
};

namespace EquationSystem
{

EquationSystemResult linear(const SquareMatrix& as, const std::vector<double>& bs);

} // EquationSystem

}
