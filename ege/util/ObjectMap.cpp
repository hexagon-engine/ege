/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#include "ObjectMap.h"

#include "PointerUtils.h"

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

std::shared_ptr<Object> ObjectMap::copy() const
{
    return make<ObjectMap>(*this);
}

const std::shared_ptr<Object>& ObjectMap::addObject(std::string name, const std::shared_ptr<Object>& subObject)
{
    if(!subObject)
        return subObject;

    auto& ref = m_subObjects[name];
    ref = subObject;
    return ref;
}

const std::shared_ptr<Object>& ObjectMap::addFloat(std::string name, ObjectFloat::ValueType value)
{
    return addObject(name, make<ObjectFloat>(value));
}

const std::shared_ptr<Object>& ObjectMap::addInt(std::string name, ObjectInt::ValueType value, ObjectInt::Type type)
{
    return addObject(name, make<ObjectInt>(value, type));
}

const std::shared_ptr<Object>& ObjectMap::addUnsignedInt(std::string name, ObjectUnsignedInt::ValueType value, ObjectUnsignedInt::Type type)
{
    return addObject(name, make<ObjectUnsignedInt>(value, type));
}

const std::shared_ptr<Object>& ObjectMap::addList(std::string name, ObjectList::ValueType value)
{
    return addObject(name, make<ObjectList>(value));
}

const std::shared_ptr<Object>& ObjectMap::addString(std::string name, ObjectString::ValueType value)
{
    return addObject(name, make<ObjectString>(value));
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

ObjectMap::ValueType::const_iterator ObjectMap::begin() const
{
    return m_subObjects.begin();
}

ObjectMap::ValueType::const_iterator ObjectMap::end() const
{
    return m_subObjects.end();
}

size_t ObjectMap::size() const
{
    return m_subObjects.size();
}

std::map<std::string, std::shared_ptr<Object>> ObjectMap::asMap() const
{
    return m_subObjects;
}

std::shared_ptr<ObjectMap> ObjectMap::merge(std::shared_ptr<ObjectMap> other)
{
    auto me = std::dynamic_pointer_cast<ObjectMap>(copy());

    if(other)
    {
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
    }

    return std::dynamic_pointer_cast<ObjectMap>(me);
}

}
