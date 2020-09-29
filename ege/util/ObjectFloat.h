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
    typedef double ValueType;

    ObjectFloat(ValueType num = 0.0)
    : m_number(num) {}

    virtual std::string toString() const;

    // TODO: int representations
    virtual std::string asString() const { return std::to_string(m_number); }
    virtual long long asInt() const { return m_number; }
    virtual bool asBool() const { return m_number; }
    virtual double asFloat() const { return m_number; }

    virtual bool isString() const { return true; }
    virtual bool isInt() const { return std::floor(m_number) == m_number; }
    virtual bool isBool() const { return true; }
    virtual bool isFloat() const { return true; }

    void setNumber(double number)
    {
        m_number = number;
    }

    virtual std::shared_ptr<Object> copy() const;

private:
    double m_number;
};

}
