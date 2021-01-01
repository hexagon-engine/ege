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

#pragma once

#include "Object.h"

#include <ege/util/PointerUtils.h>

namespace EGE
{

template<class T>
class ObjectArray : public Object
{
public:
    typedef T ElementType;
    typedef Vector<ElementType> ValueType;

    explicit ObjectArray(const ValueType& els = {})
    : m_elements(els) {}

    size_t getSize()
    {
        return m_elements.size();
    }

    ElementType getElement(size_t index)
    {
        ASSERT(index < m_elements.size());
        return m_elements[index];
    }

    virtual String toString() const
    {
        String str = "A[";
        size_t counter = 0;
        for(auto it: *this)
        {
            str += std::to_string(it);

            if(counter != m_elements.size() - 1)
            {
                str += ",";
            }
            counter++;
        }
        str += "]";
        return str;
    }

    virtual SharedPtr<Object> copy() const
    {
        return make<ObjectArray<ElementType>>(*this);
    }

    typename ValueType::const_iterator begin() const
    {
        return m_elements.begin();
    }

    typename ValueType::const_iterator end() const
    {
        return m_elements.end();
    }

private:
    Vector<ElementType> m_elements;
};

}
