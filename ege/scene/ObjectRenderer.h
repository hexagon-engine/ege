/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#pragma once

#include <memory>
#include <SFML/Graphics.hpp>

#include <ege/gfx/RenderStates.h>
#include <ege/gfx/Renderable.h>

namespace EGE
{

class SceneObject;

class ObjectRenderer : public Renderable
{
public:
    ObjectRenderer(SceneObject& sceneObject)
    : m_sceneObject(sceneObject) {}

protected:
    SceneObject& m_sceneObject;
    friend class SceneObject;
};

}
