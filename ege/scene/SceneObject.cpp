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

#include "SceneObject.h"

#include "Scene.h"

#include <ege/debug/Dump.h>
#include <ege/util/ObjectString.h>
#include <ege/util/Vector.h>
#include <ege/util/VectorOperations.h>

namespace EGE
{

SceneObject::~SceneObject()
{
    ege_log.debug() << "SceneObject::~SceneObject() " << this;
}


Vec3d SceneObject::getPosition() const
{
    if(m_parent)
    {
        return VectorOperations::rotateYawPitchRoll(m_position + m_parent->getPosition(),
                                                    -m_parent->getYaw(),
                                                    -m_parent->getPitch(),
                                                    -m_parent->getRoll(), 
                                                    m_parent->getPosition());
    }
    return m_position;
}

Vec3d SceneObject::getMotion() const
{
    if(m_parent)
        return m_parent->getMotion() + m_motion;
    return m_motion;
}

double SceneObject::getYaw() const
{
    if(m_parent)
    {
        if(m_yawMode == RotationMode::Inherit)
            return m_yaw + m_parent->getYaw();
    }
    return m_yaw;
}

double SceneObject::getPitch() const
{
    if(m_parent)
    {
        if(m_pitchMode == RotationMode::Inherit)
            return m_pitch + m_parent->getPitch();
    }
    return m_pitch;
}

double SceneObject::getRoll() const
{
    if(m_parent)
    {
        if(m_rollMode == RotationMode::Inherit)
            return m_roll + m_parent->getRoll();
    }
    return m_roll;
}


bool SceneObject::moveTo(Vec3d pos)
{
    // TODO: collisions
    setPosition(pos);
    return true;
}

bool SceneObject::flyTo(Vec3d toPos, double time, std::function<double(double)> easing)
{
    // TODO: collisions
    auto animation = make<Vec3Animation>(*this, EGE::Time(time));
    animation->setEasingFunction(easing);
    animation->addKeyframe(0, getPosition());
    animation->addKeyframe(1, toPos);
    animation->setCallback([this, toPos](std::string, EGE::Timer*) {
        if(!moveTo(toPos))
        {
            ege_log.debug() << "SceneObject2D collided during flyTo finalizing";
        }
    });
    addAnimation<Vec3d>(animation, [this](Vec3Animation&, Vec3d val) {
        if(!moveTo(val))
        {
            ege_log.debug() << "SceneObject2D collided during flyTo";
        }
    });
    return true;
}


SharedPtr<ObjectMap> SceneObject::serializeMain() const
{
    SharedPtr<ObjectMap> data = make<ObjectMap>();

    if(m_parent)
        data->addString("parent", m_parent->getName());
    data->addInt("layer", m_renderLayer);
    data->addObject("p", Serializers::fromVector3(m_position));
    data->addObject("m", Serializers::fromVector3(m_motion));
    data->addFloat("yaw", m_yaw);
    data->addFloat("pitch", m_pitch);
    data->addFloat("roll", m_roll);
    data->addUnsignedInt("yawMode", (MaxUint)m_yawMode);
    data->addUnsignedInt("pitchMode", (MaxUint)m_pitchMode);
    data->addUnsignedInt("rollMode", (MaxUint)m_rollMode);
    return data;
}

bool SceneObject::deserializeMain(SharedPtr<ObjectMap> object)
{
    ASSERT(object);
    m_position = Serializers::toVector3(object->getObject("p").to<ObjectMap>().valueOr({}));
    m_motion = Serializers::toVector3(object->getObject("m").to<ObjectMap>().valueOr({}));
    m_yaw = object->getObject("yaw").asFloat().valueOr(0);
    m_pitch = object->getObject("pitch").asFloat().valueOr(0);
    m_roll = object->getObject("roll").asFloat().valueOr(0);
    m_yawMode = (RotationMode)object->getObject("yawMode").asUnsignedInt().valueOr(m_yawMode);
    m_pitchMode = (RotationMode)object->getObject("pitchMode").asUnsignedInt().valueOr(m_pitchMode);
    m_rollMode = (RotationMode)object->getObject("rollMode").asUnsignedInt().valueOr(m_rollMode);
    m_parentId = object->getObject("parent").asString().valueOr("");
    m_renderLayer = object->getObject("layer").asInt().valueOr(0);
    return true;
}

void SceneObject::onUpdate(long long tickCounter)
{
    if(!m_parentId.empty() && !m_parent)
    {
        setParent(getOwner().getObjectByName(m_parentId));
        m_parentId = "";
        m_changedSinceLoad = false;
    }

    // Update position basing on object's motion.
    // Don't notify client because it already knows about the motion
    // and can update it on its side.
    // TODO: fps/tps scaling! m_motion should be in pxs/SECOND
    if(m_motion != Vec3d())
    {
        moveTo(getPosition() + m_motion);
        setGeometryNeedUpdate();
    }

    Animatable::onUpdate();
}

void SceneObject::doRender(Renderer& renderer, const RenderStates& states)
{
    Renderable::doRender(renderer, states);

    // Render all parts :)
    for(auto it: m_partsByLayer)
        it.second->doRender(renderer, states);
}

SharedPtr<ObjectMap> SceneObject::serialize() const
{
    SharedPtr<ObjectMap> data = make<ObjectMap>();
    data->addString("name", m_name);
    data->addObject("m", serializeMain());
    data->addObject("x", serializeExtended());
    return data;
}

bool SceneObject::deserialize(SharedPtr<ObjectMap> object)
{
    ASSERT(object);
    m_deserialized = true;
    auto _name = object->getObject("name");
    auto _m = object->getObject("m").to<ObjectMap>();
    auto _x = object->getObject("x").to<ObjectMap>();

    m_name = _name.asString().valueOr(m_name);

    bool s = true;

    if(_m.hasValue())
        s &= deserializeMain(_m.value());
    else
    {
        ege_log.error() << "No main data key in SceneObject data!";
        return false;
    }

    if(_x.hasValue())
        s &= deserializeExtended(_x.value());
    else
        // That are not required
        ege_log.verbose() << "No extended data key in SceneObject data!";

    m_changedSinceLoad = false;
    return s;
}

SharedPtr<ObjectMap> SceneObject::serializeExtended() const
{
    return nullptr;
}

bool SceneObject::deserializeExtended(SharedPtr<ObjectMap>)
{
    // nothing
    return true;
}

void SceneObject::setParent(SceneObject* object)
{
    isnSetParent(object);
    ege_log.debug() << "SceneObject::setParent(" << object << ")";
    if(m_parent)
        m_parent->m_children.erase(this);

    setMainChanged();
    m_parent = object;
    if(!object)
        return;

    m_parent->m_children.insert(this);
}

void SceneObject::addPart(String name, SharedPtr<Part> part)
{
    if(!part)
        return;
    m_parts.insert(std::make_pair(name, part));
    m_partsByLayer.insert(std::make_pair(part->getRenderLayer(), part.get()));
}

Part* SceneObject::getPart(String name)
{
    auto it = m_parts.find(name);
    if(it != m_parts.end())
        return it->second.get();
    return nullptr;
}

void SceneObject::init()
{
    ASSERT_WITH_MESSAGE(getType(), "Type not assigned to SceneObject. Use Scene::addNewObject<>() to create objects");
    getType()->fillObjectWithData(*this, !m_deserialized);
    onInit();
}

}
