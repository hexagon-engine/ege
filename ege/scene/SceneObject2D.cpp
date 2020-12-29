/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#include "SceneObject2D.h"

#include <ege/scene/Scene.h>
#include <ege/util/ObjectFloat.h>
#include <ege/util/ObjectSerializers.h>
#include <ege/util/Types.h>

namespace EGE
{

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
    auto animation = make<Animation>(this, EGE::Time(time, EGE::Time::Unit::Seconds));
    animation->setEasingFunction(easing);
    animation->addKeyframe(0,0);
    animation->addKeyframe(1,1);
    animation->setCallback([this, toPos](std::string, EGE::Timer* timer) {
                    SceneObject2D* obj = (SceneObject2D*)timer->getLoop();
                    if(!obj->moveTo(toPos))
                    {
                        DBG(1, "SceneObject2D collided during flyTo finalizing");
                    }
                 });
    Vec2d prevPos = getPosition();
    addAnimation(animation, [prevPos, this, time, toPos](EGE::Animation* anim, double val) {
                    SceneObject2D* obj = (SceneObject2D*)anim->getLoop();
                    Vec2d diff = toPos - prevPos;
                    Vec2d pos = Vec2d(val * diff.x, val * diff.y) + prevPos;
                    DUMP(1, val);
                    if(!obj->moveTo(pos))
                    {
                        DBG(1, "SceneObject2D collided during flyTo");
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
        renderer.renderRectangle(rect.left + 1, rect.top + 1, rect.width - 2, rect.height - 2, sf::Color::Transparent, sf::Color::Cyan);
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

    auto superData = SceneObject::serializeMain();

    if(superData)
        return superData->merge(data);
    else
        return data;
}

bool SceneObject2D::deserializeMain(std::shared_ptr<ObjectMap> object)
{
    // TODO: change it to ObjectSerializers!
    m_position = Serializers::toVector2(object->getObject("p").to<ObjectMap>().value());
    m_origin = Serializers::toVector2(object->getObject("o").to<ObjectMap>().value());
    m_scale = Serializers::toVector2(object->getObject("s").to<ObjectMap>().value());
    m_motion = Serializers::toVector2(object->getObject("m").to<ObjectMap>().value());
    m_rotation = object->getObject("rot").as<Float>().valueOr(0);

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
