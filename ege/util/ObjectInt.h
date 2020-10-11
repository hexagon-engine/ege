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
    typedef long long ValueType;

    enum class Type
    {
        Byte,  // 1B
        Short, // 2B
        Int,   // 4B
        Long   // 8B
    };

    ObjectInt(ValueType num = 0, Type type = Type::Long)
    : m_number(num), m_type(type) {}

    virtual std::string toString() const;

    // TODO: int representations
    virtual std::string asString() const { return std::to_string(m_number); }
    virtual long long asInt() const;
    virtual unsigned long long asUnsignedInt() { return std::max(0LL, asInt()); }
    virtual bool asBool() const { return m_number; }
    virtual double asFloat() const { return asInt(); }

    virtual bool isString() const { return true; }
    virtual bool isInt() const { return true; }
    virtual bool isUnsignedInt() const { return m_number >= 0; }
    virtual bool isBool() const { return true; }
    virtual bool isFloat() const { return true; }

    void setNumber(ValueType number)
    {
        m_number = number;
    }

    void setType(Type type)
    {
        m_type = type;
    }

    virtual std::shared_ptr<Object> copy() const;

private:
    std::string suffix() const;

    ValueType m_number;
    Type m_type;
};

}
