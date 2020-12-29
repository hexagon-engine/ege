/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#include "CameraObject2D.h"

#include <SFML/Graphics.hpp>

namespace EGE
{

void CameraObject2D::render(Renderer& renderer) const
{
    if constexpr(CAMERA_DEBUG)
    {
        sf::VertexArray varr(sf::LineStrip, 5);
        sf::Vector2f pos(getPosition().x, getPosition().y);
        varr.append(sf::Vertex(sf::Vector2f(-30.f, -30.f) + pos, sf::Color::Yellow));
        varr.append(sf::Vertex(sf::Vector2f(30.f, 30.f) + pos, sf::Color::Yellow));
        varr.append(sf::Vertex(sf::Vector2f(-30.f, 30.f) + pos, sf::Color::Yellow));
        varr.append(sf::Vertex(sf::Vector2f(30.f, -30.f) + pos, sf::Color::Yellow));
        varr.append(sf::Vertex(sf::Vector2f(-30.f, -30.f) + pos, sf::Color::Yellow));
        renderer.getTarget().draw(varr);
    }
}

}
