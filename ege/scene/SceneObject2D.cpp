/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#include "SceneObject2D.h"

#include <ege/util/ObjectInt.h>

namespace EGE
{

bool SceneObject2D::moveTo(sf::Vector2f pos)
{
    // TODO: collisions
    setPosition(pos);
    //DUMP(1, m_name);
    //DUMP(1, m_position.x);
    //DUMP(1, m_position.y);
    return true;
}

bool SceneObject2D::flyTo(sf::Vector2f toPos, double time, std::function<double(double)> easing)
{
    // TODO: collisions
    auto animation = std::make_shared<Animation>(this, EGE::Time(time, EGE::Time::Unit::Seconds));
    animation->setEasingFunction(easing);
    animation->addKeyframe(0,0);
    animation->addKeyframe(1,1);
    animation->setCallback([this, toPos](std::string name, EGE::Timer* timer) {
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

void SceneObject2D::render(sf::RenderTarget& target) const
{
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

std::shared_ptr<ObjectMap> SceneObject2D::serializeMain()
{
    std::shared_ptr<ObjectMap> data = std::make_shared<ObjectMap>();

    // TODO: change it to floats!
    data->addObject("pX", std::make_shared<ObjectInt>(m_position.x));
    data->addObject("pY", std::make_shared<ObjectInt>(m_position.y));

    data->addObject("oX", std::make_shared<ObjectInt>(m_origin.x));
    data->addObject("oY", std::make_shared<ObjectInt>(m_origin.y));

    data->addObject("sX", std::make_shared<ObjectInt>(m_scale.x));
    data->addObject("sY", std::make_shared<ObjectInt>(m_scale.y));

    data->addObject("mX", std::make_shared<ObjectInt>(m_motion.x));
    data->addObject("mY", std::make_shared<ObjectInt>(m_motion.y));

    data->addObject("rot", std::make_shared<ObjectInt>(m_rotation));

    auto superData = SceneObject::serializeMain();

    if(superData)
        return superData->merge(data);
    else
        return data;
}

#define DESERIALIZE_OBJECT_WITH_CHECK(from,name,varName,type) \
{ \
auto tmp = from->getObject(name); \
DUMP(1, name); \
DUMP(1, !tmp.expired()); \
DUMP(1, tmp.lock()->is##type()); \
DUMP(1, varName); \
if(!tmp.expired() && tmp.lock()->is##type()) \
    varName = tmp.lock()->as##type(); \
DUMP(1, varName); \
}

void SceneObject2D::deserializeMain(std::shared_ptr<ObjectMap> object)
{
    DESERIALIZE_OBJECT_WITH_CHECK(object, "pX", m_position.x, Int);
    DESERIALIZE_OBJECT_WITH_CHECK(object, "pY", m_position.y, Int);

    DESERIALIZE_OBJECT_WITH_CHECK(object, "oX", m_origin.x, Int);
    DESERIALIZE_OBJECT_WITH_CHECK(object, "oY", m_origin.y, Int);

    DESERIALIZE_OBJECT_WITH_CHECK(object, "sX", m_scale.x, Int);
    DESERIALIZE_OBJECT_WITH_CHECK(object, "sY", m_scale.y, Int);

    DESERIALIZE_OBJECT_WITH_CHECK(object, "mX", m_motion.x, Int);
    DESERIALIZE_OBJECT_WITH_CHECK(object, "mY", m_motion.y, Int);

    DESERIALIZE_OBJECT_WITH_CHECK(object, "rot", m_rotation, Int);

    SceneObject::deserializeMain(object);
}

void SceneObject2D::onUpdate(long long tickCounter)
{
    SceneObject::onUpdate(tickCounter);
    moveTo(getPosition() + m_motion);
}

}
