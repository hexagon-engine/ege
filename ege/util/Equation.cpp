#include "Equation.h"

#include <cmath>

namespace EGE
{

LinearEquation::LinearEquation(Vec2d pa, Vec2d pb)
{
    double dx = pb.x - pa.x;
    double dy = pb.y - pa.y;
    m_a = dy / dx;
    m_b = (pa.x * pb.y - pa.y * pb.x) / (pa.x - pb.x);
}

EquationResult LinearEquation::solve() const
{
    if(m_a == 0)
    {
        //std::cerr << "linear: static equation" << std::endl;
        // Static equation: b = 0
        if(m_b == 0)
            return { EquationResult::Infinite };
        else
            return {};
    }
    else
    {
        //std::cerr << "linear: 1 solution" << std::endl;
        return -m_b / m_a;
    }
}

EquationResult SquareEquation::solve() const
{
    if(m_a == 0)
    {
        //std::cerr << "square: linear equation" << std::endl;
        // Linear equation: bx+c
        return LinearEquation(m_b, m_c).solve();
    }

    double d = delta();
    //std::cerr << "square: delta=" << d << std::endl;

    if(d < 0)
        return {};
    else if(d == 0) // -b/2a
        return -m_b / (2 * m_a);
    else
    {
        //std::cerr << "square: 2 solutions" << std::endl;
        double sqrt_delta = sqrt(d);
        return {{ (-m_b - sqrt_delta) / (2 * m_a), (-m_b + sqrt_delta) / (2 * m_a) }};
    }
}

double SquareEquation::delta() const
{
    return m_b * m_b - 4 * m_a * m_c;
}

namespace Equation
{

EquationResult linear(double a, double b)
{
    return LinearEquation(a, b).solve();
}

EquationResult square(double a, double b, double c)
{
    return SquareEquation(a, b, c).solve();
}

} // Equation

}
