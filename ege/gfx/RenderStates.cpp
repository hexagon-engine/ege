/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#include "RenderStates.h"

namespace EGE
{

bool operator==(const sf::RenderStates& _1, const sf::RenderStates& _2)
{
    return _1.blendMode == _2.blendMode && _1.shader == _2.shader && _1.texture == _2.texture && _1.transform == _2.transform;
}

}
