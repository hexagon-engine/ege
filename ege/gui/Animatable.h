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

#include <ege/gui/Animation.h>
#include <ege/core/EventLoop.h>
#include <functional>
#include <memory>

namespace EGE
{

class Animatable : public EventLoop
{
public:
    Animatable(InspectorNode* parent = nullptr, String id = "EventLoop")
    : EventLoop(parent, id) {}

    template<class T>
    void addAnimation(SharedPtr<Animation<T>> animation, std::function<void(Animation<T>&, T)> callback, std::string name = "*")
    {
        if(!animation->getUpdateCallback())
        {
            animation->setUpdateCallback([callback](std::string, Timer* timer) {
                Animation<T>* anim = (Animation<T>*)timer;
                double time = (timer->getElapsedTime().getValue()) / (timer->getInterval().getValue());
                if(time < 0.0)
                    return;
                T val = anim->getValue(time);
                DUMP(ANIMATION_DEBUG, val);
                callback(*anim, val);
            });
        }

        addTimer("EGE::Animatable Delay: " + name, make<Timer>(*this, Timer::Mode::Limited, animation->getDelay(), [animation, name, this](std::string, Timer*) {
            addTimer("EGE::Animatable: " + name, animation);
        }));
    }

    void removeAnimations(std::string name);
};

}
