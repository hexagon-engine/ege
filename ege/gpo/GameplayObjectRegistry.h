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

#include <string>
#include <vector>

#include <ege/util.h>

#define EGE_GPOREGISTRY_TEMPLATE template<class IdT, class ObjT>

namespace EGE
{

enum class RegistryError : int
{
    Unknown = -1,
    ObjectExists = -2,
    NumericIdExists = -3
};

// TODO: be Serializable

// Gameplay Object Registry.
//
// Requirements for IdT:
//
// * must have IdT::IdT() <default constructor>
// * must have copy constructor or assigment operator
// * must have operator==(const IdT&, const IdT&)
// The best for it will be number primitive type e.g. int.
//
// The ObjT:
// * SHOULD be derived from GameplayObject
EGE_GPOREGISTRY_TEMPLATE
class GameplayObjectRegistry
{
public:
    struct IdTEntry
    {
        IdT baseId;
        size_t numericId;
    };

    // TODO: binary search of gpos?
    typedef std::vector<std::pair<IdTEntry, EGE::UniquePtr<ObjT>>> ArrayType;

    GameplayObjectRegistry();

    // non-copyable
    GameplayObjectRegistry(const GameplayObjectRegistry<IdT, ObjT>& obj) = delete;

    virtual ~GameplayObjectRegistry();

    // Deallocates and removes all elements from registry.
    virtual void clear();

    // Adds a new object to registry.
    // Returns negative value if object exists (or another error occured),
    // positive otherwise.
    // If %numeric is not specified or equal 0, the ID is automatically
    // generated and the return value is an ID of object.
    virtual RegistryError add(IdT id, EGE::UniquePtr<ObjT> obj, size_t numeric = 0);

    // Returns an object that has specified base %id. Returns NULL if
    // the object doesn't exist or another error occured.
    virtual ObjT* findById(const IdT& id) const;

    // Returns an object that has specified numeric %id. Returns NULL if
    // the object doesn't exist or another error occured.
    virtual ObjT* findByNumericId(const size_t id) const;

    // Returns an ID of %obj. Returns IdT() if object doesn't exist
    // or another error occured.
    virtual IdTEntry getIdOf(ObjT* obj) const;

    // Removes an object with specified %id.
    virtual void remove(const IdT& id);

    // Removes an object with specified %id.
    virtual void removeByNumericId(const size_t id);

    // Removes (but not deallocates) object with the same pointer
    // that specified.
    virtual void remove(ObjT* obj);

    // Returns count of objects registered in this registry.
    virtual int count() const;

    // Returns a reference to internal array. Can be useful for
    // iterating on elements.
    virtual const ArrayType& arr() const;

    virtual typename ArrayType::iterator begin()
    {
        return m_objects.begin();
    }
    virtual typename ArrayType::iterator end()
    {
        return m_objects.end();
    }

private:
    ArrayType m_objects;
    size_t m_greatestNumericId;
};

// Specialization for void - do not deallocate!!
template<class IdT>
class GameplayObjectRegistry<IdT, void>
{
public:
    struct IdTEntry
    {
        IdT baseId;
        size_t numericId;
    };

    typedef std::vector<std::pair<IdTEntry, void*>> ArrayType;

    // Deallocates and removes all elements from registry.
    virtual void clear();

    // Removes an object with specified %id.
    virtual void removeByNumericId(const size_t id);

    // Removes an object with specified %id.
    virtual void remove(const IdT& id);

    // Removes (but not deallocates) object with the same pointer
    // that specified.
    virtual void remove(void* obj);

    virtual typename ArrayType::iterator begin()
    {
        return m_objects.begin();
    }
    virtual typename ArrayType::iterator end()
    {
        return m_objects.end();
    }

private:
    ArrayType m_objects;
    IdT m_greatestNumericId;
};

}

#include "GameplayObjectRegistry.inl"
