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

namespace EGE
{

class ObjectInt : public Object
{
public:
    typedef MaxInt ValueType;

    enum class Type
    {
        Byte,  // 1B
        Short, // 2B
        Int,   // 4B
        Long   // 8B
    };

    explicit ObjectInt(ValueType num = 0, Type type = Type::Long)
    : m_number(num), m_type(type) {}

    virtual String toString() const;

    // TODO: int representations
    virtual String asString() const { return std::to_string(m_number); }
    virtual MaxInt asInt() const;
    virtual MaxUint asUnsignedInt() const { return std::max(0LL, asInt()); }
    virtual Boolean asBool() const { return m_number; }
    virtual MaxFloat asFloat() const { return (MaxFloat)asInt(); }

    virtual bool isString() const { return true; }
    virtual bool isInt() const { return true; }
    virtual bool isUnsignedInt() const { return m_number >= 0; }
    virtual bool isBool() const { return true; }
    virtual bool isFloat() const { return (MaxFloat)m_number == m_number; }

    void setNumber(ValueType number)
    {
        m_number = number;
    }

    void setType(Type type)
    {
        m_type = type;
    }

    Type getType() const
    {
        return m_type;
    }

    virtual SharedPtr<Object> copy() const;

private:
    String suffix() const;

    ValueType m_number;
    Type m_type;
};

}
