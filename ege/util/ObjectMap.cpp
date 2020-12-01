/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#include "ObjectMap.h"

#include "PointerUtils.h"
#include "Serializable.h"

#include <ege/main/Config.h>

namespace EGE
{

ObjectMap::ObjectMap(const ObjectMap& map)
{
    for(auto pr: map)
    {
        if(pr.second)
            addObject(pr.first, pr.second->copy());
        else
            addObject(pr.first, nullptr);
    }
}

SharedPtr<Object> ObjectMap::copy() const
{
    return make<ObjectMap>(*this);
}

const SharedPtr<Object>& ObjectMap::addObject(std::string name, const SharedPtr<Object>& subObject)
{
    auto& ref = m_subObjects[name];
    ref = subObject;
    return ref;
}

const SharedPtr<Object>& ObjectMap::addObject(String name, const Serializable& subObject)
{
    return addObject(name, subObject.serialize());
}

const SharedPtr<Object>& ObjectMap::addFloat(std::string name, ObjectFloat::ValueType value)
{
    return addObject(name, make<ObjectFloat>(value));
}

const SharedPtr<Object>& ObjectMap::addInt(std::string name, ObjectInt::ValueType value, ObjectInt::Type type)
{
    return addObject(name, make<ObjectInt>(value, type));
}

const SharedPtr<Object>& ObjectMap::addUnsignedInt(std::string name, ObjectUnsignedInt::ValueType value, ObjectUnsignedInt::Type type)
{
    return addObject(name, make<ObjectUnsignedInt>(value, type));
}

const SharedPtr<Object>& ObjectMap::addList(std::string name, ObjectList::ValueType value)
{
    return addObject(name, make<ObjectList>(value));
}

const SharedPtr<Object>& ObjectMap::addString(std::string name, ObjectString::ValueType value)
{
    return addObject(name, make<ObjectString>(value));
}

ObjectMap::_Object ObjectMap::getObject(std::string name) const
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

        if(pr.second)
            str += pr.second->toString();
        else
            str += "null";

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

std::map<std::string, SharedPtr<Object>> ObjectMap::asMap() const
{
    return m_subObjects;
}

SharedPtr<ObjectMap> ObjectMap::merge(SharedPtr<ObjectMap> other)
{
    auto me = Object::cast<ObjectMap>(copy()).value();

    if(other)
    {
        for(auto it: *other)
        {
            auto eo1 = me->getObject(it.first);
            auto existingObject = eo1.to<ObjectMap>();
            if(it.second->isMap() && existingObject.hasValue())
            {
                me->addObject(it.first, existingObject.value()->merge(Object::cast<ObjectMap>(it.second->copy()).value()));
            }
            else if(!existingObject.hasValue())
            {
                // Take only first object.
                me->addObject(it.first, it.second->copy());
            }
        }
    }

    return Object::cast<ObjectMap>(me).value();
}

template<>
Optional<MaxInt> ObjectMap::_Object::as<MaxInt>() const
{ return m_object && m_object->isInt() ? m_object->asInt() : Optional<MaxInt>(); }

template<>
Optional<MaxUint> ObjectMap::_Object::as<MaxUint>() const
{ return m_object && m_object->isUnsignedInt() ? m_object->asUnsignedInt() : Optional<MaxUint>(); }

template<>
Optional<Float> ObjectMap::_Object::as<Float>() const
{ return m_object && m_object->isFloat() ? m_object->asFloat() : Optional<Float>(); }

template<>
Optional<String> ObjectMap::_Object::as<String>() const
{ return m_object && m_object->isString() ? m_object->asString() : Optional<String>(); }

template<>
Optional<Boolean> ObjectMap::_Object::as<Boolean>() const
{ return m_object && m_object->isBool() ? m_object->asBool() : Optional<Boolean>(); }

template<>
Optional<SharedPtrVector<Object>> ObjectMap::_Object::as<SharedPtrVector<Object>>() const
{ return m_object && m_object->isList() ? m_object->asList() : Optional<SharedPtrVector<Object>>(); }

template<>
Optional<SharedPtrStringMap<Object>> ObjectMap::_Object::as<SharedPtrStringMap<Object>>() const
{ return m_object && m_object->isMap() ? m_object->asMap() : Optional<SharedPtrStringMap<Object>>(); }

}
