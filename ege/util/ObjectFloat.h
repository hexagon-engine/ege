/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
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
    virtual Boolean asBool() const { return m_number; }
    virtual MaxFloat asFloat() const { return m_number; }

    virtual bool isString() const { return true; }
    virtual bool isInt() const { return (MaxInt)m_number == m_number; }
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
