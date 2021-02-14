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

    const SharedPtr<Object>& addObject(String name, const SharedPtr<Object>& subObject);
    const SharedPtr<Object>& addObject(String name, const Serializable& subObject);

    // Set object value or add it.
    void setObject();

    const SharedPtr<Object>& addFloat(String name, ObjectFloat::ValueType value = 0.0);
    const SharedPtr<Object>& addInt(String name, ObjectInt::ValueType value = 0, ObjectInt::Type type = ObjectInt::Type::Long);
    const SharedPtr<Object>& addUnsignedInt(String name, ObjectUnsignedInt::ValueType value = 0, ObjectUnsignedInt::Type type = ObjectUnsignedInt::Type::Long);
    const SharedPtr<Object>& addList(String name, ObjectList::ValueType value = {});
    const SharedPtr<Object>& addString(String name, ObjectString::ValueType value = "");

    // TODO: exceptions
    class _Object
    {
    public:
        _Object(SharedPtr<Object> object = nullptr)
        : m_object(object) {}

        Optional<MaxInt> asInt() const
        { return m_object && m_object->isInt() ? m_object->asInt() : Optional<MaxInt>(); }

        Optional<MaxUint> asUnsignedInt() const
        { return m_object && m_object->isUnsignedInt() ? m_object->asUnsignedInt() : Optional<MaxUint>(); }

        Optional<Float> asFloat() const
        { return m_object && m_object->isFloat() ? m_object->asFloat() : Optional<Float>(); }

        Optional<String> asString() const
        { return m_object && m_object->isString() ? m_object->asString() : Optional<String>(); }

        Optional<Boolean> asBoolean() const
        { return m_object && m_object->isBool() ? m_object->asBool() : Optional<Boolean>(); }

        template<class T>
        Optional<SharedPtr<T>> to() const { return Object::cast<T>(m_object); }

        template<class T>
        bool isInstanceOf() const { return to<T>().hasValue(); }

        SharedPtr<Object> object() const { return m_object; }
        bool exists() const { return m_object.get(); }

    private:
        SharedPtr<Object> m_object;
    };

    _Object getObject(String name) const;
    bool hasObject(String name) const;

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
