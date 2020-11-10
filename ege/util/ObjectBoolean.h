/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#pragma once

#include "Object.h"

namespace EGE
{

class ObjectBoolean : public Object
{
public:
    typedef Boolean ValueType;

    explicit ObjectBoolean(ValueType num = false)
    : m_value(num) {}

    virtual String toString() const;

    // TODO: int representations
    virtual String asString() const { return m_value ? "1" : "0"; }
    virtual MaxInt asInt() const { return m_value; };
    virtual MaxUint asUnsignedInt() { return m_value; }
    virtual Boolean asBool() const { return m_value; }
    virtual MaxFloat asFloat() const { return asInt(); }

    virtual bool isString() const { return false; }
    virtual bool isInt() const { return false; }
    virtual bool isUnsignedInt() const { return false; }
    virtual bool isBool() const { return true; }
    virtual bool isFloat() const { return false; }

    void setValue(ValueType value)
    {
        m_value = value;
    }

    virtual SharedPtr<Object> copy() const;

private:
    String suffix() const;

    ValueType m_value;
};

}
