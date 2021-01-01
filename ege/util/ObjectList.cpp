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
