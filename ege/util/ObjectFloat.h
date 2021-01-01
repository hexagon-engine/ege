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

#include "Object.h"

#include <cmath>

namespace EGE
{

class ObjectFloat : public Object
{
public:
    typedef MaxFloat ValueType;

    explicit ObjectFloat(ValueType num = 0.0)
    : m_number(num) {}

    virtual String toString() const;

    // TODO: int representations
    virtual String asString() const { return std::to_string(m_number); }
    virtual MaxInt asInt() const { return m_number; }
    virtual MaxUint asUnsignedInt() const { return std::max((MaxFloat)0.0, m_number); }
    virtual Boolean asBool() const { return m_number; }
    virtual MaxFloat asFloat() const { return m_number; }

    virtual bool isString() const { return true; }
    virtual bool isInt() const { return (MaxInt)m_number == m_number; }
    virtual bool isUnsignedInt() const { return isInt() && m_number >= 0; }
    virtual bool isBool() const { return true; }
    virtual bool isFloat() const { return true; }

    void setNumber(MaxFloat number)
    {
        m_number = number;
    }

    virtual std::shared_ptr<Object> copy() const;

private:
    MaxFloat m_number;
};

}
