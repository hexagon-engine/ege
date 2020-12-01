/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#pragma once

#include "Optional.h"
#include "PointerUtils.h"
#include "Types.h"

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
    virtual String toString() const { return "<empty Object>"; };

    virtual String asString() const { return ""; }
    virtual MaxInt asInt() const { return 0; }
    virtual MaxUint asUnsignedInt() const { return asInt(); }
    virtual Boolean asBool() const { return false; }
    virtual SharedPtrStringMap<Object> asMap() const { return {}; }
    virtual MaxFloat asFloat() const { return 0.0; }
    virtual SharedPtrVector<Object> asList() const { return {}; }

    virtual bool isString() const { return false; }
    virtual bool isInt() const { return false; }
    virtual bool isUnsignedInt() const { return false; }
    virtual bool isBool() const { return false; }
    virtual bool isMap() const { return false; }
    virtual bool isFloat() const { return false; }
    virtual bool isList() const { return false; }

    virtual SharedPtr<Object> copy() const { return make<Object>(); }

    template<class T>
    static Optional<SharedPtr<T>> cast(SharedPtr<Object> object) { return std::dynamic_pointer_cast<T>(object); }
};

}
