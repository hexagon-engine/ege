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
    ObjectString(std::string str)
    : m_string(str) {}

    virtual std::string toString() const;

    // TODO: int representations
    virtual std::string asString() { return m_string; }
    virtual bool asBool() { return m_string == "true"; }

    virtual bool isString() { return true; }
    virtual bool isBool() { return true; }

    virtual std::shared_ptr<Object> copy() const;

    size_t length()
    {
        return m_string.size();
    }

private:
    std::string m_string;
};

}
