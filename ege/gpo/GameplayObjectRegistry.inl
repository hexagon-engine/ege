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

// don't include GameplayObjectRegistry.h
// because THIS file is included by GameplayObjectRegistry.h

#include <ege/main/Config.h>
#include <ege/util/PointerUtils.h>

namespace EGE
{

EGE_GPOREGISTRY_TEMPLATE
void GameplayObjectRegistry<IdT, ObjT>::clear()
{
    ASSERT_WITH_MESSAGE(!m_frozen, "Cannot modify GPO registry if it's frozen");
    m_objects.clear();
    m_objectsByBaseId.clear();
    m_objectsByNumericId.clear();
    m_greatestNumericId = 0;
}

EGE_GPOREGISTRY_TEMPLATE
RegistryError GameplayObjectRegistry<IdT, ObjT>::add(const IdT& id, UniquePtr<ObjT> obj, IdType numeric)
{
    ASSERT_WITH_MESSAGE(!m_frozen, "Cannot modify GPO registry if it's frozen");
    if(numeric != 0)
    {
        // Check if object already exists, if so, return error.
        if(m_objectsByNumericId.find(numeric) != m_objectsByNumericId.end())
            return RegistryError::NumericIdExists;
        if(m_objectsByBaseId.find(id) != m_objectsByBaseId.end())
            return RegistryError::ObjectExists;

        // Try to add object.
        try
        {
            m_objects.push_back(std::move(obj));
            m_objectsByNumericId.insert(std::make_pair(numeric, --m_objects.end()));
            m_objectsByBaseId.insert(std::make_pair(id, --m_objects.end()));
        }
        catch(...)
        {
            CRASH_WITH_MESSAGE("Exception in GameplayObjectRegistry::add()");
        }

        if(m_greatestNumericId < numeric)
            m_greatestNumericId = numeric;
        return (RegistryError)numeric;
    }
    else
    {
        // Generate ID and add the object if %numeric == 0.
        numeric = m_greatestNumericId + 1;
        return (RegistryError)add(id, std::move(obj), numeric);
    }
}

EGE_GPOREGISTRY_TEMPLATE
void GameplayObjectRegistry<IdT, ObjT>::replace(const IdT& id, UniquePtr<ObjT> obj)
{
    ASSERT_WITH_MESSAGE(!m_frozen, "Cannot modify GPO registry if it's frozen");
    auto it = m_objectsByBaseId.find(id);
    if(it == m_objectsByBaseId.end())
        return;
    *it->second = std::move(obj);
}

EGE_GPOREGISTRY_TEMPLATE
ObjT* GameplayObjectRegistry<IdT, ObjT>::findById(const IdT& id) const
{
    ASSERT_WITH_MESSAGE(m_frozen, "Registry is not frozen");
    auto it = m_objectsByBaseId.find(id);
    if(it == m_objectsByBaseId.end())
        return nullptr;
    return (*it->second).get();
}

EGE_GPOREGISTRY_TEMPLATE
ObjT* GameplayObjectRegistry<IdT, ObjT>::findByNumericId(const IdType id) const
{
    ASSERT_WITH_MESSAGE(m_frozen, "Registry is not frozen");
    auto it = m_objectsByNumericId.find(id);
    if(it == m_objectsByNumericId.end())
        return nullptr;
    return (*it->second).get();
}

EGE_GPOREGISTRY_TEMPLATE
int GameplayObjectRegistry<IdT, ObjT>::count() const
{
    return m_objects.size();
}

}
