/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
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
