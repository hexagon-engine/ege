/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#pragma once

#include "Object.h"

namespace EGE
{

class ObjectInt : public Object
{
public:
    ObjectInt(long long num)
    : m_number(num) {}

    virtual std::string toString() const;

    // TODO: int representations
    virtual std::string asString() const { return std::to_string(m_number); }
    virtual long long asInt() const { return m_number; }
    virtual bool asBool() const { return m_number; }

    virtual bool isString() const { return true; }
    virtual bool isInt() const { return true; }
    virtual bool isBool() const { return true; }

    void setNumber(long long number)
    {
        m_number = number;
    }

    virtual std::shared_ptr<Object> copy() const;

private:
    long long m_number;
};

}
