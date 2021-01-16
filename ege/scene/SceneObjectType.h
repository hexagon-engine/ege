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

#include <ege/debug/Dump.h>
#include <ege/debug/Logger.h>
#include <ege/gpo/GameplayObject.h>
#include <ege/util/Types.h>

#include "Part.h"

namespace EGE
{

class Scene;
class Scene2D;
class SceneObject2D;

class SceneObjectType : public GameplayObject
{
public:
    SceneObjectType(String typeId)
    : GameplayObject(typeId) {}

    void addPart(String name, SharedPtr<Part> part) { m_parts.insert(std::make_pair(name, part)); }
    virtual SharedPtr<SceneObject> createObject(Scene&) const { return nullptr; }

    virtual bool deserialize(SharedPtr<ObjectMap> data)
    {
        // TODO: implement that!
        log(LogLevel::Verbose) << "Loading SceneObjectType: " << getId();
        printObject(data);
        return true;
    }

protected:
    SharedPtrStringMap<Part> m_parts;
};

class SceneObjectType2D : public SceneObjectType
{
public:
    SceneObjectType2D(String typeId)
    : SceneObjectType(typeId) {}

    virtual SharedPtr<SceneObject> createObject(Scene& scene) const override;
    virtual SharedPtr<SceneObject2D> create2DObject(Scene2D& scene) const;
};

#define EGE_SCENEOBJECT2D(_class, _typeId) \
class Type : public EGE::SceneObjectType2D \
{ \
public: \
    Type() : EGE::SceneObjectType2D(_typeId) {}\
    virtual EGE::SharedPtr<EGE::SceneObject2D> create2DObject(EGE::Scene2D& scene) const { return make<_class>(scene); } \
}; \
static EGE::SharedPtr<EGE::SceneObjectType> type() { static auto type = make<Type>(); return type; } \
virtual EGE::SceneObjectType* getType() const override { return type().get(); } \

}
