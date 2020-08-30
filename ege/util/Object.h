/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#pragma once

#include <memory>
#include <string>

namespace EGE
{

class ObjectMap;

class Object
{
public:
    virtual std::string toString() const = 0;

    virtual std::string asString() { return ""; }
    virtual int asInt() { return 0; }
    virtual bool asBool() { return false; }
    virtual std::shared_ptr<ObjectMap> asMap() { return nullptr; }

    virtual bool isString() { return false; }
    virtual bool isInt() { return false; }
    virtual bool isBool() { return false; }
    virtual bool isMap() { return false; }

    virtual std::shared_ptr<Object> copy() const = 0;

};

}
