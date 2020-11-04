/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

// don't include GameplayObjectRegistry.h
// because THIS file is included by GameplayObjectRegistry.h

#include <ege/main/Config.h>
#include <ege/util/PointerUtils.h>

namespace EGE
{

EGE_GPOREGISTRY_TEMPLATE
GameplayObjectRegistry<IdT, ObjT>::GameplayObjectRegistry()
{
    m_greatestNumericId = 0;
}

EGE_GPOREGISTRY_TEMPLATE
GameplayObjectRegistry<IdT, ObjT>::~GameplayObjectRegistry()
{
    clear();
}

EGE_GPOREGISTRY_TEMPLATE
void GameplayObjectRegistry<IdT, ObjT>::clear()
{
    for(auto it = m_objects.begin(); it != m_objects.end(); it++)
    {
        delete it->second;
    }
    m_objects.clear();
    m_greatestNumericId = 0;
}

EGE_GPOREGISTRY_TEMPLATE
RegistryError GameplayObjectRegistry<IdT, ObjT>::add(IdT id, ObjT* obj, size_t numeric)
{
    if(numeric != 0)
    {
        // TODO: optimize it
        // Check if object already exists, if so, return error.
        for(auto it = m_objects.begin(); it != m_objects.end(); it++)
        {
            if(it->first.baseId == id)
                return RegistryError::ObjectExists;
            if(it->first.numericId == numeric)
                return RegistryError::NumericIdExists;
        }

        // Try to add object.
        try
        {
            m_objects.push_back(std::make_pair(IdTEntry{id,numeric}, obj));
        }
        catch(std::exception& e)
        {
            std::cerr << "000C EGE/gpo: std::exception caught in GameplayObjectRegistry::add(): " << e.what() << std::endl;
            return RegistryError::Unknown;
        }
        catch(...)
        {
            std::cerr << "000D EGE/gpo: unknown exception caught in GameplayObjectRegistry::add()" << std::endl;
            return RegistryError::Unknown;
        }

        if(m_greatestNumericId < numeric)
            m_greatestNumericId = numeric;
        return (RegistryError)numeric;
    }
    else
    {
        // Generate ID and add the object if %numeric == 0.
        numeric = m_greatestNumericId + 1;
        return (RegistryError)add(id, obj, numeric);
    }
}

EGE_GPOREGISTRY_TEMPLATE
ObjT* GameplayObjectRegistry<IdT, ObjT>::findById(const IdT& id) const
{
    for(auto it = m_objects.begin(); it != m_objects.end(); it++)
    {
        if(it->first.baseId == id)
            return it->second;
    }
    return nullptr;
}

EGE_GPOREGISTRY_TEMPLATE
ObjT* GameplayObjectRegistry<IdT, ObjT>::findByNumericId(const size_t id) const
{
    for(auto it = m_objects.begin(); it != m_objects.end(); it++)
    {
        if(it->first.numericId == id)
            return it->second;
    }
    return nullptr;
}

EGE_GPOREGISTRY_TEMPLATE
typename GameplayObjectRegistry<IdT, ObjT>::IdTEntry GameplayObjectRegistry<IdT, ObjT>::getIdOf(ObjT* obj) const
{
    for(auto it = m_objects.begin(); it != m_objects.end(); it++)
    {
        if(it->second == obj)
            return it->first;
    }
    return IdTEntry{IdT(), 0};
}

EGE_GPOREGISTRY_TEMPLATE
void GameplayObjectRegistry<IdT, ObjT>::remove(const IdT& id)
{
    for(auto it = m_objects.begin(); it != m_objects.end(); it++)
    {
        if(it->first.baseId == id)
        {
            delete it->second;
            m_objects.erase(it);
            return;
        }
    }
}

EGE_GPOREGISTRY_TEMPLATE
void GameplayObjectRegistry<IdT, ObjT>::removeByNumericId(const size_t id)
{
    for(auto it = m_objects.begin(); it != m_objects.end(); it++)
    {
        if(it->first.numericId == id)
        {
            delete it->second;
            m_objects.erase(it);
            return;
        }
    }
}

EGE_GPOREGISTRY_TEMPLATE
void GameplayObjectRegistry<IdT, ObjT>::remove(ObjT* obj)
{
    for(auto it = m_objects.begin(); it != m_objects.end(); it++)
    {
        if(it->second == obj)
        {
            delete it->second;
            m_objects.erase(it);
            return;
        }
    }
}

EGE_GPOREGISTRY_TEMPLATE
int GameplayObjectRegistry<IdT, ObjT>::count() const
{
    return m_objects.size();
}

EGE_GPOREGISTRY_TEMPLATE
const typename GameplayObjectRegistry<IdT, ObjT>::ArrayType& GameplayObjectRegistry<IdT, ObjT>::arr() const
{
    return m_objects;
}

// VOID

// don't deallocate void objects
template<class IdT>
void GameplayObjectRegistry<IdT, void>::clear()
{
    m_objects.clear();
}
template<class IdT>
void GameplayObjectRegistry<IdT, void>::remove(const IdT& id)
{
    for(auto it = m_objects.begin(); it != m_objects.end(); it++)
    {
        if(it->first.baseId == id)
        {
            m_objects.erase(it);
            return;
        }
    }
}
template<class IdT>
void GameplayObjectRegistry<IdT, void>::removeByNumericId(const size_t id)
{
    for(auto it = m_objects.begin(); it != m_objects.end(); it++)
    {
        if(it->first.numericId == id)
        {
            m_objects.erase(it);
            return;
        }
    }
}
template<class IdT>
void GameplayObjectRegistry<IdT, void>::remove(void* obj)
{
    for(auto it = m_objects.begin(); it != m_objects.end(); it++)
    {
        if(it->second == obj)
        {
            m_objects.erase(it);
            return;
        }
    }
}

}
