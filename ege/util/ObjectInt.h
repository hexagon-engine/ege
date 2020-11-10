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
    virtual MaxUint asUnsignedInt() { return std::max(0LL, asInt()); }
    virtual Boolean asBool() const { return m_number; }
    virtual MaxFloat asFloat() const { return asInt(); }

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
