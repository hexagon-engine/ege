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
    explicit Scene2D(GUIGameLoop* loop)
    : Scene(loop) {}

    void setCamera(std::weak_ptr<CameraObject2D> camera);
    std::weak_ptr<CameraObject2D> getCamera() { return m_camera; }
    virtual void renderOnly(sf::RenderTarget& target, const RenderStates& states);

    virtual sf::View getView(sf::RenderTarget& target);
    virtual sf::Vector2f mapScreenToScene(sf::RenderTarget& target, sf::Vector2i screenPos);
    virtual sf::Vector2i mapSceneToScreen(sf::RenderTarget& target, sf::Vector2f scenePos);

protected:
    std::weak_ptr<CameraObject2D> m_camera;
};

}
