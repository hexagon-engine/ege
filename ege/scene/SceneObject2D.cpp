/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#include "SceneObject2D.h"

namespace EGE
{

bool SceneObject2D::moveTo(sf::Vector2f pos)
{
    // TODO: collisions
    m_position = pos;
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

void SceneObject2D::onUpdate(long long tickCounter)
{
    SceneObject::onUpdate(tickCounter);
    moveTo(getPosition() + m_motion);
}

}
