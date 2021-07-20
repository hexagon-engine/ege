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
#include <ege/debug/InspectorNode.h>
#include <ege/debug/Logger.h>
#include <ege/gfx/RenderStates.h>
#include <ege/gfx/Renderable.h>
#include <ege/gui/Animatable.h>
#include <ege/gui/AnimationEasingFunctions.h>
#include <ege/util/Serializable.h>
#include <ege/util/Rect.h>

#define EGE_SCENEOBJECT(typeId) \
    static EGE::String type() { return typeId; }

namespace EGE
{

class Scene;

class SceneObject : public Animatable, public Renderable, public Serializable
{
public:
    SceneObject(Scene& owner)
    : Animatable((InspectorNode*)&owner, "SceneObject"), m_owner(owner) {}

    enum Type
    {
        Static,
        Dynamic
    };

    virtual ~SceneObject();

    virtual void onUpdate(long long tickCounter);
    virtual void doRender(Renderer& renderer, const RenderStates& states = {}) override;
    virtual void render(Renderer&) const override {}

    bool isDead() const { return m_dead; }

    UidType getObjectId() const { return m_id; }
    void setObjectId(UidType id) { if(!m_id) m_id = id; }

    std::string getName() const { return m_name; }
    void setName(std::string name) { m_name = name; setChanged(); }

    virtual SharedPtr<ObjectMap> serialize() const override;
    virtual bool deserialize(SharedPtr<ObjectMap>) override;

    virtual SharedPtr<ObjectMap> serializeMain() const;
    virtual bool deserializeMain(SharedPtr<ObjectMap>);

    virtual SharedPtr<ObjectMap> serializeExtended() const;
    virtual bool deserializeExtended(SharedPtr<ObjectMap>);

    // Clears main and extended changed flags. Called by server.
    // FIXME: it should be only callable by Server.

    void clearMainChangedFlag() { m_mainChanged = false; }
    void clearExtendedChangedFlag() { m_extendedChanged = false; }

    bool getMainChangedFlag() const { return m_mainChanged; }
    bool getExtendedChangedFlag() const { return m_extendedChanged; }
    bool didChangeSinceLoad() const { return m_changedSinceLoad; }

    void setDead() { m_dead = true; }
    Scene& getOwner() const { return m_owner; }

    void setParent(SceneObject* object);

    void addPart(String name, SharedPtr<Part> part);
    SharedPtrStringMap<Part>& getParts() { return m_parts; }
    Part* getPart(String name);

    virtual SharedPtr<SceneObjectType> getType() const { return m_type; }
    void setType(SharedPtr<SceneObjectType> type) { if(!m_type) m_type = type; }

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

    virtual String isnInfo() const override { return m_type->getId() + ": " + m_name; }

    virtual bool allowSave() const { return true; }

    void setPosition(Vec3d position) { m_position = position; }
    Vec3d getPosition() const;

    void setMotion(Vec3d motion) { m_motion = motion; }

    // This motion is absolute (relative to scene, NOT to parent).
    Vec3d getMotion() const;

    virtual RectD getBoundingBox() const { return RectD({m_position.x, m_position.y}, {}); }

    enum RotationMode
    {
        Inherit, // Object angle is parent angle + own angle
        Lock     // Parent angle is ignored
    };

    void setYawRotationMode(RotationMode mode) { m_yawMode = mode; setGeometryNeedUpdate(); }
    void setPitchRotationMode(RotationMode mode) { m_pitchMode = mode; setGeometryNeedUpdate(); }
    void setRollRotationMode(RotationMode mode) { m_rollMode = mode; setGeometryNeedUpdate(); }
    void setRotationMode(RotationMode mode) { m_yawMode = mode; setGeometryNeedUpdate(); }

    void setYaw(double value) { m_yaw = value; setGeometryNeedUpdate(); }
    void setPitch(double value) { m_pitch = value; setGeometryNeedUpdate(); }
    void setRoll(double value) { m_roll = value; setGeometryNeedUpdate(); }

    // Alias for setYaw in 2D coordinates.
    void setRotation(double value) { m_yaw = value; setGeometryNeedUpdate(); }

    double getYaw() const;
    double getPitch() const;
    double getRoll() const;

    // Alias for getYaw in 2D coordinates.
    double getRotation() const { return getYaw(); }

    bool moveTo(Vec3d targetPos);
    bool flyTo(Vec3d targetPos, double time, std::function<double(double)> easing = AnimationEasingFunctions::linear);

protected:
    friend class Scene;

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
    int m_renderLayer = 0;

    Set<SceneObject*> m_children;
    SceneObject* m_parent = nullptr;
    String m_parentId;
    Type m_parentType = Type::Dynamic;
    SharedPtr<SceneObjectType> m_type;

private:
    SharedPtrStringMap<Part> m_parts;
    std::multimap<int, Part*> m_partsByLayer;

    Vec3d m_position;
    Vec3d m_motion;
    double m_yaw = 0;
    double m_pitch = 0;
    double m_roll = 0;
    RotationMode m_yawMode = RotationMode::Inherit;
    RotationMode m_pitchMode = RotationMode::Inherit;
    RotationMode m_rollMode = RotationMode::Inherit;
    bool m_deserialized = false;
};

}
