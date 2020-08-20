/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#pragma once

#include "CameraObject2D.h"
#include "Scene.h"

#include <memory>
#include <SFML/Graphics.hpp>

namespace EGE
{

class Scene2D : public Scene
{
public:
    Scene2D(Widget* parent)
    : Scene(parent) {}

    void setCamera(std::weak_ptr<CameraObject2D> camera);
    virtual void renderOnly(sf::RenderTarget& target);

protected:
    std::weak_ptr<CameraObject2D> m_camera;
};

}
