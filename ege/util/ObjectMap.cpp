/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#include "ObjectMap.h"

#include <ege/main/Config.h>

namespace EGE
{

ObjectMap::ObjectMap(const ObjectMap& map)
{
    for(auto pr: map)
    {
        addObject(pr.first, pr.second->copy());
    }
}

std::shared_ptr<Object>& ObjectMap::addObject(std::string name, std::shared_ptr<Object> subObject)
{
    ASSERT(subObject);
    auto& ref = m_subObjects[name];
    ref = subObject;
    return ref;
}

std::weak_ptr<Object> ObjectMap::getObject(std::string name)
{
    return m_subObjects[name];
}

std::string ObjectMap::toString() const
{
    std::string str = "{";
    size_t counter = 0;
    for(auto pr: *this)
    {
        std::string key = pr.first;
        str += "\"" + key + "\":";
        str += pr.second->toString();

        if(counter != m_subObjects.size() - 1)
        {
            str += ",";
        }
        counter++;
    }
    str += "}";
    return str;
}

ObjectMap::InternalMap::const_iterator ObjectMap::begin() const
{
    return m_subObjects.begin();
}

ObjectMap::InternalMap::const_iterator ObjectMap::end() const
{
    return m_subObjects.end();
}

size_t ObjectMap::size() const
{
    return m_subObjects.size();
}

std::shared_ptr<ObjectMap> ObjectMap::asMap() const
{
    return std::make_shared<ObjectMap>(*this);
}

}
