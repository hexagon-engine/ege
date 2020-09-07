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
    if(!subObject)
        return subObject;

    auto& ref = m_subObjects[name];
    ref = subObject;
    return ref;
}

std::weak_ptr<Object> ObjectMap::getObject(std::string name) const
{
    auto it = m_subObjects.find(name);
    if(it != m_subObjects.end())
        return it->second;
    return {};
}

bool ObjectMap::hasObject(std::string name) const
{
    auto it = m_subObjects.find(name);
    if(it != m_subObjects.end())
        return true;
    return false;
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

std::shared_ptr<ObjectMap> ObjectMap::merge(std::shared_ptr<ObjectMap> other)
{
    auto me = std::dynamic_pointer_cast<ObjectMap>(copy());

    for(auto it: *other)
    {
        auto eo1 = me->getObject(it.first);
        auto existingObject = !eo1.expired() ? std::dynamic_pointer_cast<ObjectMap>((std::shared_ptr<Object>)eo1) : nullptr;
        if(it.second->isMap() && existingObject)
        {
            me->addObject(it.first, existingObject->merge(std::dynamic_pointer_cast<ObjectMap>(it.second->copy())));
        }
        else if(!existingObject)
        {
            // Take only first object.
            me->addObject(it.first, it.second->copy());
        }
    }

    return std::dynamic_pointer_cast<ObjectMap>(me);
}

}
