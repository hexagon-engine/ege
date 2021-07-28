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
*    Copyright (c) Sppmacd 2021
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

#include <ege/util/Types.h>
#include <ege/main/Config.h>
#include <initializer_list>

namespace EGE
{

template<class T, size_t S>
class Matrix
{
public:
    static inline constexpr size_t Size = S;

    Matrix() = default;

    static Matrix identity()
    {
        Matrix result;
        for(size_t s = 0; s < S; s++)
            result.item(s, s) = 1;
        return result;
    }

    Matrix(Matrix const& other)
    {
        *this = other;
    }

    template<class U>
    Matrix(Matrix<U, S> const& other)
    {
        *this = other;
    }

    // Input is in row-major order (it's more natural in notation)
    template<class U>
    Matrix(std::initializer_list<U> data)
    {
        *this = data;
    }

    template<class U>
    Matrix& operator=(Matrix<U, S> const& other)
    {
        for(size_t x = 0; x < S; x++)
        {
            for(size_t y = 0; y < S; y++)
            {
                item(x, y) = other.item(x, y);
            }
        }
        return *this;
    }

    Matrix& operator=(Matrix const& other)
    {
        if(this == &other)
            return *this;
        return operator=<T>(other);
    }

    template<class U>
    Matrix& operator=(std::initializer_list<U> data)
    {
        ASSERT(data.size() == S * S);
        for(size_t x = 0; x < S; x++)
        {
            for(size_t y = 0; y < S; y++)
            {
                // Input is in row-major order (it's more natural in notation)
                item(x, y) = data.begin()[y*S+x];
            }
        }
        return *this;
    }

    // Internal data are in column-major order to match OpenGL.
    T const* column(size_t index) const { ASSERT(index < S); return m_data[index]; }
    T* column(size_t index) { ASSERT(index < S); return m_data[index]; }
    T& item(size_t _column, size_t _row) { ASSERT(_column < S && _row < S); return column(_column)[_row]; }
    T const& item(size_t _column, size_t _row) const { ASSERT(_column < S && _row < S); return column(_column)[_row]; }

    Matrix operator+(const Matrix& other)
    {
        Matrix result;
        for(size_t x = 0; x < S; x++)
        {
            for(size_t y = 0; y < S; y++)
            {
                result.item(x, y) = item(x, y) + other.item(x, y);
            }
        }
        return result;
    }

    Matrix operator*(const Matrix& other) const
    {
        // FIXME: Don't be naive
        Matrix result;
        for(size_t i = 0; i < S; i++)
        {
            for(size_t j = 0; j < S; j++)
            {
                double val = 0;
                for(size_t oc = 0; oc < S; oc++)
                {
                    val += item(oc, i) * other.item(j, oc);
                }
                result.item(j, i) = val;
            }
        }
        return result;
    }

    Matrix& operator*=(const Matrix& other)
    {
        return *this = *this * other;
    }

    Matrix transposed() const
    {
        Matrix result;
        for(size_t x = 0; x < S; x++)
        {
            for(size_t y = 0; y < S; y++)
            {
                result.item(y, x) = item(x, y);
            }
        }
        return result;
    }

    // Internal data are in column-major order to match OpenGL.
    T const* data() const { return reinterpret_cast<T const*>(m_data); }
    T* data() { return reinterpret_cast<T*>(m_data); }

private:
    T m_data[S][S] {};
};

using FloatMatrix4x4 = Matrix<float, 4>;
using DoubleMatrix4x4 = Matrix<double, 4>;

}
