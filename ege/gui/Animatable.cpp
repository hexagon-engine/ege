/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#include "Animatable.h"

#include <ege/loop/Timer.h>

namespace EGE
{

void Animatable::addAnimation(std::shared_ptr<Animation> animation, std::function<void(Animation*,double)> callback, std::string name)
{
    if(!animation->getUpdateCallback())
    {
        animation->setUpdateCallback([callback](std::string name, Timer* timer) {
                                    Animation* anim = (Animation*)timer;
                                    double time = (timer->getElapsedTime().getValue()) / (timer->getInterval().getValue());
                                    if(time < 0.0)
                                        return;
                                    double val = anim->getValue(time);
                                    DUMP(ANIMATION_DEBUG, val);
                                    callback(anim, val);
                                });
    }

    Timer* timer = new Timer(this, Timer::Mode::Limited, animation->getDelay());
    timer->setCallback([animation, this](std::string name, Timer* timer) { addTimer("EGE::Animatable: " + name, animation); });
    addTimer("EGE::Animatable Delay: " + name, timer);
}

void Animatable::removeAnimations(std::string name)
{
    removeTimer("EGE::Animatable Delay: " + name);
    removeTimer("EGE::Animatable: " + name);
}

}
