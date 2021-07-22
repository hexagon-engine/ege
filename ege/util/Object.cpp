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

#include "Object.h"
#include "ObjectList.h"
#include "ObjectMap.h"

namespace EGE
{

ObjectValue ObjectValue::_ListIterator::operator*() const
{
    return ObjectValue(*m_iterator);
}

ObjectValue::_ListIterator ObjectValue::_ListWrapper::begin() const { return m_list->begin(); }
ObjectValue::_ListIterator ObjectValue::_ListWrapper::end() const { return m_list->end(); }

ObjectValue ObjectValue::_ListWrapper::get(Size offset) const
{
    return m_list->getObject(offset);
}

std::pair<String, ObjectValue> ObjectValue::_MapIterator::operator*() const
{
    return std::make_pair(m_iterator->first, ObjectValue(m_iterator->second));
}

ObjectValue::_MapIterator ObjectValue::_MapWrapper::begin() const { return m_map->begin(); }
ObjectValue::_MapIterator ObjectValue::_MapWrapper::end() const { return m_map->end(); }

ObjectValue ObjectValue::_MapWrapper::get(const String& key) const
{
    return m_map->get(key);
}

// TODO: Generalize these to allow other list/map-like objects than ObjectList/Map
Optional<ObjectValue::_ListWrapper> ObjectValue::asList() const
{
    return m_object && m_object->isList() ? _ListWrapper(Object::unsafeCast<ObjectList>(m_object)) : Optional<_ListWrapper>();
}

Optional<ObjectValue::_MapWrapper> ObjectValue::asMap() const
{
    return m_object && m_object->isMap() ? _MapWrapper(Object::unsafeCast<ObjectMap>(m_object)) : Optional<_MapWrapper>();
}

}

std::ostream& operator<<(std::ostream& _str, const EGE::Object& data)
{
    return _str << data.toString();
}
