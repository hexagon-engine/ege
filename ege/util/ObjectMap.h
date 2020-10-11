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

    typedef std::map<std::string, std::shared_ptr<Object>> ValueType;

    const std::shared_ptr<Object>& addObject(std::string name, const std::shared_ptr<Object>& subObject);
    const std::shared_ptr<Object>& addFloat(std::string name, ObjectFloat::ValueType value = 0.0);
    const std::shared_ptr<Object>& addInt(std::string name, ObjectInt::ValueType value = 0, ObjectInt::Type type = ObjectInt::Type::Long);
    const std::shared_ptr<Object>& addUnsignedInt(std::string name, ObjectUnsignedInt::ValueType value = 0, ObjectUnsignedInt::Type type = ObjectUnsignedInt::Type::Long);
    const std::shared_ptr<Object>& addList(std::string name, ObjectList::ValueType value = {});
    const std::shared_ptr<Object>& addString(std::string name, ObjectString::ValueType value = "");

    std::weak_ptr<Object> getObject(std::string name) const;
    bool hasObject(std::string name) const;

    virtual std::string toString() const;

    ValueType::const_iterator begin() const;
    ValueType::const_iterator end() const;
    size_t size() const;

    virtual std::map<std::string, std::shared_ptr<Object>> asMap() const;
    virtual bool isMap() const { return true; }

    virtual std::shared_ptr<Object> copy() const;
    std::shared_ptr<ObjectMap> merge(std::shared_ptr<ObjectMap> other);

private:
     ValueType m_subObjects;
};

}
