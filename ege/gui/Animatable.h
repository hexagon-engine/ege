/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#pragma once

#include <ege/gui/Animation.h>
#include <ege/loop/EventLoop.h>
#include <functional>
#include <memory>

namespace EGE
{

class Animatable : public EventLoop
{
public:
    void addAnimation(std::shared_ptr<Animation> animation, std::function<void(Animation*,double)> callback);
};

}
