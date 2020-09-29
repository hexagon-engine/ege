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
    typedef std::vector<std::shared_ptr<Object>> ValueType;

    ObjectList(ValueType objects = {})
    : m_objects(objects) {}

    virtual std::string toString() const;

    virtual std::vector<std::shared_ptr<Object>> asList() const { return m_objects; }
    virtual bool isList() const { return true; }

    virtual std::shared_ptr<Object> copy() const;

    const std::shared_ptr<Object>& addObject(const std::shared_ptr<Object>& subObject);
    const std::shared_ptr<Object>& insertObject(size_t position, const std::shared_ptr<Object>& subObject);
    std::weak_ptr<Object> getObject(size_t offset) const;

    ValueType::const_iterator begin() const;
    ValueType::const_iterator end() const;
    size_t size() const;

    std::shared_ptr<ObjectList> merge(std::shared_ptr<ObjectList> other);

private:
    ValueType m_objects;
};

}
