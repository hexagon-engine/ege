/*
*
*   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*
*      ,----  ,----  ,----
*      |      |      |
*      |----  | --,  |----
*      |      |   |  |
*      '----  '---'  '----
*
*     Framework Library for Hexagon
*
*    Copyright (c) Sppmacd 2020 - 2021
*
*    Permission is hereby granted, free of charge, to any person obtaining a copy
*    of this software and associated documentation files (the "Software"), to deal
*    in the Software without restriction, including without limitation the rights
*    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
*    copies of the Software, and to permit persons to whom the Software is
*    furnished to do so, subject to the following conditions:
*
*    The above copyright notice and this permission notice shall be included in all
*    copies or substantial portions of the Software.
*
*    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
*    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
*    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
*    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
*    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
*    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
*    SOFTWARE.
*
*   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*
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
    static Optional<SharedPtr<T>> cast(SharedPtr<Object> object)
    {
        auto ptr = std::dynamic_pointer_cast<T>(object);
        if(!ptr)
            return {};
        return ptr;
    }
};

}

std::ostream& operator<<(std::ostream& _str, const EGE::Object& data);
