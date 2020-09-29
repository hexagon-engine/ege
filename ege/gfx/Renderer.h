/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#pragma once

#include <SFML/Graphics.hpp>

namespace EGE
{

class Renderer
{
public:
    Renderer(sf::RenderTarget& target)
    : m_target(target) {}

    // Common renderers.
    // For more advanced shapes, use SFML sf::Drawables.

    void renderRectangle(double x, double y, double width, double height, sf::Color color, sf::Color outlineColor = sf::Color::Transparent);

    sf::RenderTarget& getTarget()
    {
        return m_target;
    }

private:
    // noncopyable, nonmoveable
    Renderer(const Renderer& other) = delete;
    Renderer(Renderer&& other) = delete;
    Renderer& operator=(const Renderer& other) = delete;
    Renderer& operator=(Renderer&& other) = delete;

    sf::RenderTarget& m_target;
};

}
