/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#include "SceneObject2D.h"

#include <ege/scene/Scene.h>
#include <ege/util/ObjectFloat.h>
#include <ege/util/Types.h>

namespace EGE
{

bool SceneObject2D::moveTo(sf::Vector2f pos, bool notify)
{
    // TODO: collisions

    if(notify)
        setPosition(pos);
    else
    {
        m_position = pos;
        if(m_renderer)
            m_renderer->setGeometryNeedUpdate();
    }

    //DUMP(1, m_name);
    //DUMP(1, m_position.x);
    //DUMP(1, m_position.y);
    return true;
}

bool SceneObject2D::flyTo(sf::Vector2f toPos, double time, std::function<double(double)> easing)
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
    sf::Vector2f prevPos = getPosition();
    addAnimation(animation, [prevPos, this, time, toPos](EGE::Animation* anim, double val) {
                    SceneObject2D* obj = (SceneObject2D*)anim->getLoop();
                    sf::Vector2f diff = toPos - prevPos;
                    sf::Vector2f pos = sf::Vector2f(val * diff.x, val * diff.y) + prevPos;
                    DUMP(1, val);
                    if(!obj->moveTo(pos))
                    {
                        DBG(1, "SceneObject2D collided during flyTo");
                    }
                 });
    return true;
}

void SceneObject2D::render(sf::RenderTarget& target, const RenderStates& states) const
{
    SceneObject::render(target, states);

    // debug shape
    if constexpr(SCENEOBJECT2D_DEBUG)
    {
        sf::FloatRect rect(getBoundingBox());
        sf::RectangleShape rs(rect.getSize() - sf::Vector2f(2.f, 2.f));
        rs.setPosition(rect.getPosition() + sf::Vector2f(1.f, 1.f));
        rs.setOutlineColor(sf::Color::Cyan);
        rs.setOutlineThickness(1.f);
        rs.setFillColor(sf::Color::Transparent);
        target.draw(rs);
    }
}

std::shared_ptr<ObjectMap> SceneObject2D::serializeMain() const
{
    std::shared_ptr<ObjectMap> data = make<ObjectMap>();

    // TODO: change it to ObjectSerializers!
    data->addFloat("pX", m_position.x);
    data->addFloat("pY", m_position.y);

    data->addFloat("oX", m_origin.x);
    data->addFloat("oY", m_origin.y);

    data->addFloat("sX", m_scale.x);
    data->addFloat("sY", m_scale.y);

    data->addFloat("mX", m_motion.x);
    data->addFloat("mY", m_motion.y);

    data->addFloat("rot", m_rotation);

    auto superData = SceneObject::serializeMain();

    if(superData)
        return superData->merge(data);
    else
        return data;
}

bool SceneObject2D::deserializeMain(std::shared_ptr<ObjectMap> object)
{
    m_position.x = object->getObject("pX").as<Float>().valueOr(0);
    m_position.y = object->getObject("pY").as<Float>().valueOr(0);

    m_origin.x = object->getObject("oX").as<Float>().valueOr(0);
    m_origin.y = object->getObject("oY").as<Float>().valueOr(0);

    m_scale.x = object->getObject("sX").as<Float>().valueOr(0);
    m_scale.y = object->getObject("sY").as<Float>().valueOr(0);

    m_motion.x = object->getObject("mX").as<Float>().valueOr(0);
    m_motion.y = object->getObject("mY").as<Float>().valueOr(0);

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
    if(m_motion != sf::Vector2f())
    {
        moveTo(getPosition() + m_motion, false);
        if(m_renderer)
            m_renderer->setGeometryNeedUpdate();
    }
}

}
