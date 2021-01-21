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

class SceneObjectRegistry
{
public:
    typedef SharedPtrStringMap<SceneObjectType> ValueType;

    SharedPtr<SceneObjectType> getType(String typeId);

    template<class SO>
    class SimpleSceneObjectType2D : public SceneObjectType2D
    {
    public:
        SimpleSceneObjectType2D(String typeId)
        : SceneObjectType2D(typeId) {}

        virtual SharedPtr<SceneObject2D> createEmptyObject(Scene2D& scene) const override
            { return make<SO>(scene, *this); }
    };

    template<class SO>
    void addType2D()
        { addType(make<SimpleSceneObjectType2D<SO>>(SO::type())); }

    void addType(SharedPtr<SceneObjectType> type);
    bool loadFromFile(String fileName, const IOStreamConverter& converter = JSONConverter());

private:
    // Try to load parts from fileType to customType.
    void mergeTypes(SharedPtr<SceneObjectType> customType, SharedPtr<SceneObjectType> fileType);

    ValueType m_typeMap;
};

}
