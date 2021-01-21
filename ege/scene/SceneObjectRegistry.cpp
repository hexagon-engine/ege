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

#include "SceneObjectRegistry.h"

#include "SceneLoader.h"

namespace EGE
{

SharedPtr<SceneObjectType> SceneObjectRegistry::getType(String typeId)
{
    auto it = m_typeMap.find(typeId);
    if(it == m_typeMap.end())
        return nullptr;
    return it->second;
}

void SceneObjectRegistry::addType(SharedPtr<SceneObjectType> type)
{
    auto it = m_typeMap.find(type->getId());
    if(it != m_typeMap.end())
    {
        log(LogLevel::Info) << "Duplicate SceneObjectType: " << type->getId() << ". Trying to merge parts.";
        mergeTypes(type, it->second);

        // Replace old loaded-from-file type with custom type
        it->second = type;
        return;
    }
    m_typeMap.insert(std::make_pair(type->getId(), type));
}

bool SceneObjectRegistry::loadFromFile(String fileName, const IOStreamConverter& converter)
{
    return SceneLoader::loadRegistry(*this, fileName, converter);
}

void SceneObjectRegistry::mergeTypes(SharedPtr<SceneObjectType> customType, SharedPtr<SceneObjectType> fileType)
{
    ASSERT(customType && fileType);
    for(auto& part: fileType->getPartStubs())
    {
        customType->addPartStub(part.first, part.second);
    }
}

}

