/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#pragma once

#include "CameraObject.h"
#include "SceneObject2D.h"

#include <SFML/Graphics.hpp>

#define CAMERA_DEBUG 1

namespace EGE
{

class CameraObject2D : public CameraObject<sf::Vector2f>, public SceneObject2D
{
public:
    CameraObject2D(Scene* parent)
    : CameraObject(parent), SceneObject2D(parent, "Camera Object (2D)") {}

    sf::Vector2f getEyePosition()
    {
        return getPosition();
    }

    void setEyePosition(sf::Vector2f position)
    {
        setPosition(position);
    }

    virtual void render(sf::RenderTarget& target) const;
};

}
