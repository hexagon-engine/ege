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
// Requirements for IdT: same as for std::map key
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
        IdType numericId;
    };

    // TODO: binary search of gpos?
    typedef List<UniquePtr<ObjT>> ArrayType;

    GameplayObjectRegistry() = default;

    // non-copyable
    GameplayObjectRegistry(const GameplayObjectRegistry<IdT, ObjT>& obj) = delete;

    // Deallocates and removes all elements from registry.
    void clear();

    // Adds a new object to registry.
    // Returns negative value if object exists (or another error occured),
    // positive otherwise.
    // If %numeric is not specified or equal 0, the ID is automatically
    // generated and the return value is an ID of object.
    RegistryError add(const IdT& id, UniquePtr<ObjT> obj, IdType numeric = 0);

    // Replaces an object in registry.
    void replace(const IdT& id, UniquePtr<ObjT> obj);

    // Returns an object that has specified base %id. Returns NULL if
    // the object doesn't exist or another error occured.
     ObjT* findById(const IdT& id) const;

    // Returns an object that has specified numeric %id. Returns NULL if
    // the object doesn't exist or another error occured.
    ObjT* findByNumericId(const IdType id) const;

    int count() const;

    typename ArrayType::iterator begin() { return m_objects.begin(); }
    typename ArrayType::iterator end() { return m_objects.end(); }

    // Disable modifying registry. It must be done in Gameplay Object
    // Manager, before any object is accessed.
    void freeze() { m_frozen = true; }
    bool isFrozen() { return m_frozen; }

private:
    typedef Map<IdT, typename ArrayType::iterator> ArrayByIdType;
    typedef Map<IdType, typename ArrayType::iterator> ArrayByNIdType;
    typedef Map<ObjT*, typename ArrayType::iterator> ArrayByObjectType;

    ArrayType m_objects;
    ArrayByIdType m_objectsByBaseId;
    ArrayByNIdType m_objectsByNumericId;
    bool m_frozen = false;

    IdType m_greatestNumericId = 0;
};

}

#include "GameplayObjectRegistry.inl"
