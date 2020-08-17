/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#include "SceneObject.h"

#include "Scene.h"

namespace EGE
{

void SceneObject::onUpdate(long long tickCounter)
{
    (void) tickCounter;
    updateTimers();
}

}
