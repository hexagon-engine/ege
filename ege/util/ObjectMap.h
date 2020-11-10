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

#include <map>
#include <memory>
#include <string>
#include <vector>

namespace EGE
{

class ObjectMap : public Object
{
public:
    ObjectMap() = default;
    ObjectMap(const ObjectMap& map);

    typedef SharedPtrStringMap<Object> ValueType;

    const SharedPtr<Object>& addObject(String name, const SharedPtr<Object>& subObject);
    const SharedPtr<Object>& addFloat(String name, ObjectFloat::ValueType value = 0.0);
    const SharedPtr<Object>& addInt(String name, ObjectInt::ValueType value = 0, ObjectInt::Type type = ObjectInt::Type::Long);
    const SharedPtr<Object>& addUnsignedInt(String name, ObjectUnsignedInt::ValueType value = 0, ObjectUnsignedInt::Type type = ObjectUnsignedInt::Type::Long);
    const SharedPtr<Object>& addList(String name, ObjectList::ValueType value = {});
    const SharedPtr<Object>& addString(String name, ObjectString::ValueType value = "");

    WeakPtr<Object> getObject(String name) const;
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
