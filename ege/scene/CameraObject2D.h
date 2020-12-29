/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#pragma once

#include "CameraObject.h"
#include "SceneObject2D.h"

#include <ege/gfx/RenderStates.h>
#include <SFML/Graphics.hpp>

#define CAMERA_DEBUG 0

namespace EGE
{

class CameraObject2D : public CameraObject<Vec2d>, public SceneObject2D
{
public:
    CameraObject2D(std::shared_ptr<EGE::Scene> owner)
    : CameraObject(owner), SceneObject2D(owner, "EGE::CameraObject2D") {}

    Vec2d getEyePosition() { return m_following ? ((SceneObject2D*)m_following.get())->getPosition() : getPosition(); }
    void setEyePosition(Vec2d position) { setPosition(position); }

    virtual void render(Renderer& renderer) const override;
};

}
