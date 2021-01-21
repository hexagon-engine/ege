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

namespace EGE
{

class ObjectList : public Object
{
public:
    typedef SharedPtrVector<Object> ValueType;

    explicit ObjectList(ValueType objects = {})
    : m_objects(objects) {}

    virtual String toString() const;

    virtual SharedPtrVector<Object> asList() const { return m_objects; }
    virtual Boolean isList() const { return true; }

    virtual SharedPtr<Object> copy() const;

    const SharedPtr<Object>& addObject(const SharedPtr<Object>& subObject);

    const SharedPtr<Object>& insertObject(Size position, const SharedPtr<Object>& subObject);
    WeakPtr<Object> getObject(Size offset) const;

    ValueType::const_iterator begin() const;
    ValueType::const_iterator end() const;
    Size size() const;

    SharedPtr<ObjectList> merge(SharedPtr<ObjectList> other);

private:
    ValueType m_objects;
};

}
