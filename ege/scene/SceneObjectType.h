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

#include "parts/Part.h"
#include "parts/PartStub.h"

namespace EGE
{

class Scene;

class SceneObjectType : public GameplayObject
{
public:
    SceneObjectType(String typeId)
    : GameplayObject(typeId) {}

    void addPartStub(String name, PartStub partStub) { m_parts.insert(std::make_pair(name, partStub)); }

    virtual void fillObjectWithData(SceneObject&, bool applyDefaults) const;
    virtual SharedPtr<SceneObject> createEmptyObject(Scene& scene) const { return make<SceneObject>(scene); }

    virtual bool deserialize(SharedPtr<ObjectMap> data) override;
    virtual SharedPtr<ObjectMap> serialize() const final override { return nullptr; }

    virtual const StringMap<PartStub>& getPartStubs() const { return m_parts; }
    virtual SharedPtr<ObjectMap> getDefaults() const { return m_defaults; }

protected:
    StringMap<PartStub> m_parts;
    SharedPtr<ObjectMap> m_defaults;
};

}
