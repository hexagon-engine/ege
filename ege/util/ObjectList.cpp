/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#include "ObjectList.h"
#include "PointerUtils.h"

#include <ege/main/Config.h>
#include <ege/util/PointerUtils.h>

namespace EGE
{

std::shared_ptr<Object> ObjectList::copy() const
{
    return make<ObjectList>(*this);
}

std::string ObjectList::toString() const
{
    std::string str = "[";
    size_t counter = 0;
    for(auto it: *this)
    {
        str += it->toString();

        if(counter != m_objects.size() - 1)
        {
            str += ",";
        }
        counter++;
    }
    str += "]";
    return str;
}

const std::shared_ptr<Object>& ObjectList::addObject(const std::shared_ptr<Object>& subObject)
{
    m_objects.push_back(subObject);
    return subObject;
}

const std::shared_ptr<Object>& ObjectList::insertObject(size_t position, const std::shared_ptr<Object>& subObject)
{
    m_objects.insert(m_objects.begin() + position, subObject);
    return subObject;
}

std::weak_ptr<Object> ObjectList::getObject(size_t offset) const
{
    ASSERT(offset < size());
    return m_objects[offset];
}

ObjectList::ValueType::const_iterator ObjectList::begin() const
{
    return m_objects.begin();
}

ObjectList::ValueType::const_iterator ObjectList::end() const
{
    return m_objects.end();
}

size_t ObjectList::size() const
{
    return m_objects.size();
}

std::shared_ptr<ObjectList> ObjectList::merge(std::shared_ptr<ObjectList> other)
{
    std::shared_ptr<ObjectList> newList = make<ObjectList>();
    newList->m_objects.insert(newList->m_objects.begin(), begin(), end());

    // Append other list to `this' list.
    newList->m_objects.insert(newList->m_objects.end(), other->begin(), other->end());
    return newList;
}

}
