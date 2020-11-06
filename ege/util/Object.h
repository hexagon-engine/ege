/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#pragma once

#include "PointerUtils.h"

#include <map>
#include <memory>
#include <string>
#include <vector>

namespace EGE
{

class ObjectMap;

class Object
{
public:
    virtual std::string toString() const { return "<empty Object>"; };

    virtual std::string asString() const { return ""; }
    virtual long long asInt() const { return 0; }
    virtual unsigned long long asUnsignedInt() const { return asInt(); }
    virtual bool asBool() const { return false; }
    virtual std::map<std::string, std::shared_ptr<Object>> asMap() const { return {}; }
    virtual double asFloat() const { return 0.0; }
    virtual std::vector<std::shared_ptr<Object>> asList() const { return {}; }

    virtual bool isString() const { return false; }
    virtual bool isInt() const { return false; }
    virtual bool isUnsignedInt() const { return false; }
    virtual bool isBool() const { return false; }
    virtual bool isMap() const { return false; }
    virtual bool isFloat() const { return false; }
    virtual bool isList() const { return false; }

    virtual std::shared_ptr<Object> copy() const { return make<Object>(); };
};

}
