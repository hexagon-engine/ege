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

#include "SceneObject2D.h"

#include <ege/debug/Dump.h>
#include <ege/scene/Scene.h>
#include <ege/util/ObjectFloat.h>
#include <ege/util/ObjectSerializers.h>
#include <ege/util/Types.h>

namespace EGE
{

Vec2d SceneObject2D::getPosition() const
{
    if(m_parent)
    {
        SceneObject2D* parent = dynamic_cast<SceneObject2D*>(m_parent);
        if(parent)
            return VectorOperations::rotate(m_position + parent->getPosition(), -parent->getRotation(), parent->getPosition());
    }
    return m_position;
}

double SceneObject2D::getRotation() const
{
    if(m_parent)
    {
        SceneObject2D* parent = dynamic_cast<SceneObject2D*>(m_parent);
        if(parent && m_rotationMode == RotationMode::Inherit)
            return m_rotation + parent->getRotation();
    }
    return m_rotation;
}

bool SceneObject2D::moveTo(Vec2d pos, bool notify)
{
    // TODO: collisions

    if(notify)
        setPosition(pos);
    else
    {
        m_position = pos;
        setGeometryNeedUpdate();
    }

    //DUMP(1, m_name);
    //DUMP(1, m_position.x);
    //DUMP(1, m_position.y);
    return true;
}

bool SceneObject2D::flyTo(Vec2d toPos, double time, std::function<double(double)> easing)
{
    // TODO: collisions
    auto animation = make<Vec2Animation>(*this, EGE::Time(time, EGE::Time::Unit::Seconds));
    animation->setEasingFunction(easing);
    animation->addKeyframe(0, getPosition());
    animation->addKeyframe(1, toPos);
    animation->setCallback([this, toPos](std::string, EGE::Timer*) {
        if(!moveTo(toPos))
        {
            log(LogLevel::Debug) << "SceneObject2D collided during flyTo finalizing";
        }
    });
    addAnimation<Vec2d>(animation, [this](Vec2Animation&, Vec2d val) {
        if(!moveTo(val))
        {
            log(LogLevel::Debug) << "SceneObject2D collided during flyTo";
        }
    });
    return true;
}

void SceneObject2D::render(Renderer& renderer) const
{
    SceneObject::render(renderer);

    // debug shape
    if constexpr(SCENEOBJECT2D_DEBUG)
    {
        sf::FloatRect rect(getBoundingBox());
        renderer.renderRectangle(rect.left + 1, rect.top + 1, rect.width - 2, rect.height - 2, Colors::transparent, Colors::cyan);
    }
}

std::shared_ptr<ObjectMap> SceneObject2D::serializeMain() const
{
    std::shared_ptr<ObjectMap> data = make<ObjectMap>();

    data->addObject("p", Serializers::fromVector2(m_position));
    data->addObject("o", Serializers::fromVector2(m_origin));
    data->addObject("s", Serializers::fromVector2(m_scale));
    data->addObject("m", Serializers::fromVector2(m_motion));
    data->addFloat("rot", m_rotation);
    data->addUnsignedInt("rm", (MaxUint)m_rotationMode);

    auto superData = SceneObject::serializeMain();

    if(superData)
        return superData->merge(data);
    else
        return data;
}

bool SceneObject2D::deserializeMain(std::shared_ptr<ObjectMap> object)
{
    m_position = Serializers::toVector2(object->getObject("p").to<ObjectMap>().valueOr({}));
    m_origin = Serializers::toVector2(object->getObject("o").to<ObjectMap>().valueOr({}));
    m_scale = Serializers::toVector2(object->getObject("s").to<ObjectMap>().valueOr({}));
    m_motion = Serializers::toVector2(object->getObject("m").to<ObjectMap>().valueOr({}));
    m_rotation = object->getObject("rot").as<Float>().valueOr(0);
    m_rotationMode = (RotationMode)object->getObject("rm").as<MaxUint>().valueOr(m_rotationMode);

    return SceneObject::deserializeMain(object);
}

void SceneObject2D::onUpdate(long long tickCounter)
{
    SceneObject::onUpdate(tickCounter);

    // Update position basing on object's motion.
    // Don't notify client because it already knows about the motion
    // and can update it on its side.
    // TODO: fps/tps scaling! m_motion should be in pxs/SECOND
    if(m_motion != Vec2d())
    {
        moveTo(getPosition() + m_motion, false);
        setGeometryNeedUpdate();
    }
}

}
