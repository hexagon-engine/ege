/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#pragma once

#include "Scene.h"

namespace EGE
{

// requirements for CoordType:
//  must have default constructor public
template<class CoordType>
class CameraObject
{
public:
    CameraObject(Scene* parent)
    : m_parent(parent) {}

    CoordType getEyePosition() { return CoordType(); };

protected:
    Scene* m_parent;
};

}
