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

#include "ObjectRenderer.h"
#include "SceneObjectType.h"

#include <SFML/Graphics.hpp>
#include <ege/controller/Controllable.h>
#include <ege/debug/InspectorNode.h>
#include <ege/debug/Logger.h>
#include <ege/gfx/RenderStates.h>
#include <ege/gfx/Renderable.h>
#include <ege/gui/Animatable.h>
#include <ege/util/Serializable.h>

#define EGE_SCENEOBJECT(typeId) \
    static EGE::String type() { return #typeId; }

namespace EGE
{

class Scene;

class SceneObject : public Animatable, public Controllable, public Renderable
{
public:
    SceneObject(Scene& owner, const SceneObjectType& type)
    : Animatable((InspectorNode*)&owner, "SceneObject: " + type.getId()), m_owner(owner), m_type(type) {}

    enum Type
    {
        Static,
        Dynamic
    };

    virtual ~SceneObject();

    virtual void onUpdate(long long tickCounter);
    virtual void doRender(Renderer& renderer, const RenderStates& states = {});

    bool isDead() const { return m_dead; }

    UidType getObjectId() const { return m_id; }
    void setObjectId(UidType id) { if(!m_id) m_id = id; }

    std::string getName() const { return m_name; }
    void setName(std::string name) { m_name = name; setChanged(); }

    virtual std::shared_ptr<ObjectMap> serialize() const;
    virtual bool deserialize(std::shared_ptr<ObjectMap>);

    virtual std::shared_ptr<ObjectMap> serializeMain() const;
    virtual bool deserializeMain(std::shared_ptr<ObjectMap>);

    virtual std::shared_ptr<ObjectMap> serializeExtended() const;
    virtual bool deserializeExtended(std::shared_ptr<ObjectMap>);

    // Clears main and extended changed flags. Called by server.
    // FIXME: it should be only callable by Server.

    void clearMainChangedFlag() { m_mainChanged = false; }
    void clearExtendedChangedFlag() { m_extendedChanged = false; }

    bool getMainChangedFlag() const { return m_mainChanged; }
    bool getExtendedChangedFlag() const { return m_extendedChanged; }
    bool didChangeSinceLoad() const { return m_changedSinceLoad; }

    void setDead() { m_dead = true; }
    Scene& getOwner() const { return m_owner; }
    void setRenderer(std::shared_ptr<ObjectRenderer> renderer) { m_renderer = std::static_pointer_cast<ObjectRenderer>(renderer); }

    void setParent(SceneObject* object);

    void addPart(String name, SharedPtr<Part> part) { m_parts.insert(std::make_pair(name, part)); }

    SharedPtrStringMap<Part>& getParts() { return m_parts; }
    Part* getPart(String name);

    virtual const SceneObjectType& getType() const { return m_type; }

    template<class SO>
    static SharedPtr<SO> cast(SharedPtr<SceneObject> obj) { return std::dynamic_pointer_cast<SO>(obj); }

    // Called by init() function (e.g when the object is added
    // and it's sure that it's deserialized, if applicable).
    // It should be used instead of constructor. Can be used to
    // add e.g. animations.
    virtual void onInit() {}

    // The higher number is rendered on top of the lower number.
    // e.g. layer 1 objects are covered by layer 2 objects.
    int getRenderLayer() const { return m_renderLayer; }
    void setRenderLayer(int layer) { m_renderLayer = layer; }

    virtual String isnInfo() const override { return m_name; }

protected:
    void setMainChanged() { m_mainChanged = true; setChanged(); }
    void setExtendedChanged() { m_extendedChanged = true; setChanged(); }
    void setChanged() { m_changedSinceLoad = true; }

    void init();

    Scene& m_owner;
    bool m_dead = false;
    UidType m_id = 0;
    String m_name;
    bool m_mainChanged = true;
    bool m_extendedChanged = true;
    bool m_changedSinceLoad = false;
    SharedPtr<ObjectRenderer> m_renderer;
    int m_renderLayer = 0;

    Set<SceneObject*> m_children;
    SceneObject* m_parent = nullptr;
    String m_parentId;
    Type m_parentType = Type::Dynamic;
    const SceneObjectType& m_type;

    friend class ObjectRenderer;
    friend class Scene;

private:
    SharedPtrStringMap<Part> m_parts;
};

}
