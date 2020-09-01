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
    virtual std::string asString() const { return m_string; }
    virtual bool asBool() const { return m_string == "true"; }

    virtual bool isString() const { return true; }
    virtual bool isBool() const { return true; }

    virtual std::shared_ptr<Object> copy() const;

    void setString(std::string str)
    {
        m_string = str;
    }

    size_t length()
    {
        return m_string.size();
    }

private:
    std::string m_string;
};

}
