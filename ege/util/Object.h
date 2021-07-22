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

#include "Optional.h"
#include "PointerUtils.h"
#include "Types.h"

#include <map>
#include <memory>
#include <string>
#include <vector>

namespace EGE
{

class ObjectMap;
class ObjectList;

class Object
{
public:
    virtual String toString() const { return "<empty Object>"; };

    virtual String asString() const { return ""; }
    virtual MaxInt asInt() const { return 0; }
    virtual MaxUint asUnsignedInt() const { return asInt(); }
    virtual Boolean asBool() const { return false; }
    virtual SharedPtrStringMap<Object> asMap() const { return {}; }
    virtual MaxFloat asFloat() const { return 0.0; }
    virtual SharedPtrVector<Object> asList() const { return {}; }

    virtual bool isString() const { return false; }
    virtual bool isInt() const { return false; }
    virtual bool isUnsignedInt() const { return false; }
    virtual bool isBool() const { return false; }
    virtual bool isMap() const { return false; }
    virtual bool isFloat() const { return false; }
    virtual bool isList() const { return false; }

    virtual SharedPtr<Object> copy() const { return make<Object>(); }

    template<class T>
    static Optional<SharedPtr<T>> cast(SharedPtr<Object> object)
    {
        auto ptr = std::dynamic_pointer_cast<T>(object);
        if(!ptr)
            return {};
        return ptr;
    }

    template<class T>
    static SharedPtr<T> unsafeCast(SharedPtr<Object> object)
    {
        return std::static_pointer_cast<T>(object);
    }
};

class ObjectValue
{
public:
    ObjectValue(SharedPtr<Object> object = nullptr)
    : m_object(object) {}

    // T must be derived from Object
    template<class T>
    ObjectValue(SharedPtr<T> object)
    : m_object(object) {}

    explicit ObjectValue(const ObjectValue& object)
    : m_object(object.object()->copy()) {}

    // TODO: Generalize these to allow other list/map-like objects than ObjectList/Map
    class _ListIterator
    {
    public:
        ObjectValue operator*() const;
        _ListIterator& operator++() { m_iterator++; return *this; }
        bool operator==(const _ListIterator& other) const { return m_iterator == other.m_iterator; }
        bool operator!=(const _ListIterator& other) const { return m_iterator != other.m_iterator; }

    private:
        friend class ObjectValue;

        _ListIterator(SharedPtrVector<Object>::const_iterator iterator)
        : m_iterator(iterator) {}

        SharedPtrVector<Object>::const_iterator m_iterator;
    };

    class _ListWrapper
    {
    public:
        _ListWrapper() = default;
        _ListIterator begin() const;
        _ListIterator end() const;

        ObjectValue get(Size key) const;

        // TODO: Remove it once _ListWrapper gets all features of ObjectList
        SharedPtr<ObjectList> list() { return m_list; }

    private:
        friend class ObjectValue;

        explicit _ListWrapper(SharedPtr<ObjectList> list)
        : m_list(list) {}

        SharedPtr<ObjectList> m_list;
    };

    class _MapIterator
    {
    public:
        std::pair<String, ObjectValue> operator*() const;
        _MapIterator& operator++() { m_iterator++; return *this; }
        bool operator==(const _MapIterator& other) const { return m_iterator == other.m_iterator; }
        bool operator!=(const _MapIterator& other) const { return m_iterator != other.m_iterator; }

    private:
        friend class ObjectValue;

        _MapIterator(SharedPtrStringMap<Object>::const_iterator iterator)
        : m_iterator(iterator) {}
        SharedPtrStringMap<Object>::const_iterator m_iterator;
    };

    class _MapWrapper
    {
    public:
        _MapWrapper() = default;
        _MapIterator begin() const;
        _MapIterator end() const;

        ObjectValue get(const String& key) const;

        // TODO: Remove it once _MapWrapper gets all features of ObjectMap
        SharedPtr<ObjectMap> map() { return m_map; }

    private:
        friend class ObjectValue;

        explicit _MapWrapper(SharedPtr<ObjectMap> map)
        : m_map(map) {}

        SharedPtr<ObjectMap> m_map;
    };


    Optional<MaxInt> asInt() const
    { return m_object && m_object->isInt() ? m_object->asInt() : Optional<MaxInt>(); }

    Optional<MaxUint> asUnsignedInt() const
    { return m_object && m_object->isUnsignedInt() ? m_object->asUnsignedInt() : Optional<MaxUint>(); }

    Optional<Float> asFloat() const
    { return m_object && m_object->isFloat() ? m_object->asFloat() : Optional<Float>(); }

    Optional<String> asString() const
    { return m_object && m_object->isString() ? m_object->asString() : Optional<String>(); }

    Optional<Boolean> asBoolean() const
    { return m_object && m_object->isBool() ? m_object->asBool() : Optional<Boolean>(); }

    Optional<_ListWrapper> asList() const;
    Optional<_MapWrapper> asMap() const;

    template<class T>
    Optional<SharedPtr<T>> to() const { return Object::cast<T>(m_object); }

    template<class T>
    bool isInstanceOf() const { return to<T>().hasValue(); }

    SharedPtr<Object> object() const { return m_object; }
    operator SharedPtr<Object>() const { return m_object; }
    bool exists() const { return m_object.get(); }

private:
    SharedPtr<Object> m_object;
};

}

std::ostream& operator<<(std::ostream& _str, const EGE::Object& data);
