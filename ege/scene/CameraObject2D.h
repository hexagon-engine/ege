/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#pragma once

#include "CameraObject.h"
#include "SceneObject2D.h"

#include <ege/gfx/RenderStates.h>
#include <SFML/Graphics.hpp>

#define CAMERA_DEBUG 1

namespace EGE
{

class CameraObject2D : public CameraObject<sf::Vector2f>, public SceneObject2D
{
public:
    CameraObject2D(std::shared_ptr<EGE::Scene> owner)
    : CameraObject(owner), SceneObject2D(owner, "EGE::CameraObject2D") {}

    sf::Vector2f getEyePosition() { return m_following ? ((SceneObject2D*)m_following.get())->getPosition() : getPosition(); }
    void setEyePosition(sf::Vector2f position) { setPosition(position); }

    virtual void render(sf::RenderTarget& target, const RenderStates& states) const;
};

}
