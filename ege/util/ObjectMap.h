/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
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

        template<class T>
        Optional<T> as() const { CRASH(); }

        template<class T>
        bool is() const { return as<T>().hasValue(); }

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
