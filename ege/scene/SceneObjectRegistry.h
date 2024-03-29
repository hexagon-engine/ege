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

#include "SceneObjectType.h"

#include <ege/util/JSONConverter.h>
#include <ege/util/Types.h>

namespace EGE
{

class SceneObjectRegistry final
{
public:
    typedef SharedPtrStringMap<SceneObjectType> ValueType;

    SceneObjectRegistry();
    SceneObjectRegistry(const SceneObjectRegistry&) = delete;
    SceneObjectRegistry(SceneObjectRegistry&&) = delete;
    SharedPtr<SceneObjectType> getType(String typeId);

    template<class SO>
    class SimpleSceneObjectType : public SceneObjectType
    {
    public:
        SimpleSceneObjectType(String typeId)
        : SceneObjectType(typeId) {}

        virtual SharedPtr<SceneObject> createEmptyObject(Scene& scene) const override
            { return make<SO>(scene); }
    };

    template<class SO>
    void addType()
        { addType(make<SimpleSceneObjectType<SO>>(SO::type())); }

    void addType(SharedPtr<SceneObjectType> type);
    bool loadFromFile(String fileName, const IOStreamConverter& converter = JSONConverter());

private:
    // Try to load parts from fileType to customType.
    void mergeTypes(SharedPtr<SceneObjectType> customType, SharedPtr<SceneObjectType> fileType);

    ValueType m_typeMap;
};

}
