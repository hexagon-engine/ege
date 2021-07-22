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

#include "Object.h"
#include "ObjectBoolean.h"
#include "ObjectFloat.h"
#include "ObjectInt.h"
#include "ObjectList.h"
#include "ObjectString.h"
#include "ObjectUnsignedInt.h"

#include <ege/main/Config.h>
#include <map>
#include <memory>
#include <string>
#include <vector>

namespace EGE
{

class Serializable;

class ObjectMap : public Object
{
public:
    ObjectMap() = default;
    ObjectMap(const ObjectMap& map);

    typedef SharedPtrStringMap<Object> ValueType;

    SharedPtr<Object> addObject(String name, SharedPtr<Object> subObject);
    SharedPtr<Object> add(String name, SharedPtr<Object> subObject) { return addObject(name, subObject); }
    SharedPtr<Object> addObject(String name, const Serializable& subObject);
    SharedPtr<Object> add(String name, const Serializable& subObject) { return addObject(name, subObject); }

    SharedPtr<Object> addFloat(String name, ObjectFloat::ValueType value = 0.0);
    SharedPtr<Object> addInt(String name, ObjectInt::ValueType value = 0, ObjectInt::Type type = ObjectInt::Type::Long);
    SharedPtr<Object> addUnsignedInt(String name, ObjectUnsignedInt::ValueType value = 0, ObjectUnsignedInt::Type type = ObjectUnsignedInt::Type::Long);
    SharedPtr<Object> addList(String name, ObjectList::ValueType value = {});
    SharedPtr<Object> addString(String name, ObjectString::ValueType value = "");
    SharedPtr<Object> addBoolean(String name, ObjectBoolean::ValueType value = false);

    ObjectValue getObject(String name) const;
    ObjectValue get(String name) const { return getObject(name); }
    bool hasObject(String name) const;
    bool has(String name) const { return hasObject(name); };

    virtual String toString() const;

    ValueType::const_iterator begin() const;
    ValueType::const_iterator end() const;
    size_t size() const;

    virtual SharedPtrStringMap<Object> asMap() const;
    virtual bool isMap() const { return true; }

    virtual SharedPtr<Object> copy() const;
    SharedPtr<ObjectMap> merge(SharedPtr<ObjectMap> other);

private:
     ValueType m_subObjects;
};

}
