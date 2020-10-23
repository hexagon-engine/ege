/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#include "CameraObject2D.h"

#include <SFML/Graphics.hpp>

namespace EGE
{

void CameraObject2D::render(sf::RenderTarget& target, const RenderStates&) const
{
    if constexpr(CAMERA_DEBUG)
    {
        sf::VertexArray varr(sf::LineStrip, 5);
        varr.append(sf::Vertex(sf::Vector2f(-30.f, -30.f) + getPosition(), sf::Color::Yellow));
        varr.append(sf::Vertex(sf::Vector2f(30.f, 30.f) + getPosition(), sf::Color::Yellow));
        varr.append(sf::Vertex(sf::Vector2f(-30.f, 30.f) + getPosition(), sf::Color::Yellow));
        varr.append(sf::Vertex(sf::Vector2f(30.f, -30.f) + getPosition(), sf::Color::Yellow));
        varr.append(sf::Vertex(sf::Vector2f(-30.f, -30.f) + getPosition(), sf::Color::Yellow));
        target.draw(varr);
    }
}

}
