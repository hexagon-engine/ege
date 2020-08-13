/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#pragma once

#include <string>
#include <vector>

#define EGE_GPOREGISTRY_TEMPLATE template<class IdT, class ObjT>

namespace EGE
{

enum class RegistryError : int
{
    Unknown = -1,
    ObjectExists = -2,
    NumericIdExists = -3
};

// Gameplay Object Registry.
// Note that objects MUST be created dynamically
// because they are destroyed when game is closed.
//
// Requirements for IdT:
//
// * must have IdT::IdT() <default constructor>
// * must have copy constructor or assigment operator
// * must have operator==(const IdT&, const IdT&)
// The best for it will be number primitive type e.g. int.
//
// The ObjT can be anything.
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
    typedef std::vector<std::pair<IdTEntry, ObjT*>> ArrayType;

    GameplayObjectRegistry();

    virtual ~GameplayObjectRegistry();

    // Deallocates and removes all elements from registry.
    virtual void clear();

    // Adds a new object to registry. It must be created dynamically.
    // Returns negative value if object exists (or another error occured),
    // positive otherwise.
    // If %numeric is not specified or equal 0, the ID is automatically
    // generated and the return value is an ID of object.
    virtual RegistryError add(IdT id, ObjT* obj, size_t numeric = 0);

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

private:
    ArrayType m_objects;
    IdT m_greatestNumericId;
};

}

#include "GameplayObjectRegistry.inl"
