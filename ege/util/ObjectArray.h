/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
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
    typedef std::vector<ElementType> ValueType;

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

    virtual std::string toString() const
    {
        std::string str = "A[";
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

    virtual std::shared_ptr<Object> copy() const
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
    std::vector<ElementType> m_elements;
};

}
