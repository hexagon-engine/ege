/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#pragma once

#include <map>
#include <memory>
#include <string>
#include <vector>

#include "Object.h"

namespace EGE
{

class ObjectMap : public Object
{
public:
    ObjectMap() = default;
    ObjectMap(const ObjectMap& map);

    typedef std::map<std::string, std::shared_ptr<Object>> InternalMap;

    const std::shared_ptr<Object>& addObject(std::string name, const std::shared_ptr<Object>& subObject);
    std::weak_ptr<Object> getObject(std::string name) const;
    bool hasObject(std::string name) const;

    virtual std::string toString() const;

    InternalMap::const_iterator begin() const;
    InternalMap::const_iterator end() const;
    size_t size() const;

    virtual std::shared_ptr<ObjectMap> asMap() const;
    virtual bool isMap() const { return true; }

    virtual std::shared_ptr<Object> copy() const { return asMap(); }
    std::shared_ptr<ObjectMap> merge(std::shared_ptr<ObjectMap> other);

private:
     InternalMap m_subObjects;
};

}
