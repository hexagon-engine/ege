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
    return true;
}

bool SceneObject2D::flyTo(sf::Vector2f toPos, float time, std::function<double(double)> easing)
{
    // TODO: collisions
    auto animation = std::make_shared<Animation>(this, EGE::Time(time, EGE::Time::Unit::Seconds));
    animation->setEasingFunction(easing);
    animation->addKeyframe(0,0);
    animation->addKeyframe(1,1);
    sf::Vector2f prevPos = getPosition();
    addAnimation(animation, [prevPos, this, time, toPos](EGE::Animation* anim, double val) {
                    SceneObject2D* obj = (SceneObject2D*)anim;
                    sf::Vector2f diff = toPos - prevPos;
                    sf::Vector2f pos(val * diff.x, val * diff.y);
                    if(!obj->moveTo(pos))
                    {
                        DBG(1, "SceneObject2D collided during flyTo");
                    }
                 });
    return true;
}

}
