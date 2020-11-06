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
    typedef bool ValueType;

    explicit ObjectBoolean(ValueType num = false)
    : m_value(num) {}

    virtual std::string toString() const;

    // TODO: int representations
    virtual std::string asString() const { return m_value ? "1" : "0"; }
    virtual long long asInt() const { return m_value; };
    virtual unsigned long long asUnsignedInt() { return m_value; }
    virtual bool asBool() const { return m_value; }
    virtual double asFloat() const { return asInt(); }

    virtual bool isString() const { return false; }
    virtual bool isInt() const { return false; }
    virtual bool isUnsignedInt() const { return false; }
    virtual bool isBool() const { return true; }
    virtual bool isFloat() const { return false; }

    void setValue(ValueType value)
    {
        m_value = value;
    }

    virtual std::shared_ptr<Object> copy() const;

private:
    std::string suffix() const;

    ValueType m_value;
};

}
