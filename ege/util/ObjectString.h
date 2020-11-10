/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#pragma once

#include "Object.h"

namespace EGE
{

class ObjectString : public Object
{
public:
    typedef String ValueType;

    explicit ObjectString(ValueType str = "")
    : m_string(str) {}

    virtual String toString() const;

    // TODO: int representations
    virtual String asString() const { return m_string; }
    virtual bool asBool() const { return m_string == "true"; }

    virtual bool isString() const { return true; }
    virtual bool isBool() const { return true; }

    virtual SharedPtr<Object> copy() const;

    void setString(ValueType str)
    {
        m_string = str;
    }

    Size length()
    {
        return m_string.size();
    }

private:
    ValueType m_string;
};

}
