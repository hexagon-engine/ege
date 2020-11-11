/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#pragma once

#include "RenderStates.h"

#include <ege/util/Vector.h>
#include <SFML/Graphics.hpp>

namespace EGE
{

// TODO: move it to another file
struct Vertex
{
    float x = 0, y = 0, z = 0;
    char r = -128, g = -128, b = -128, a = 127;
    float texX = 0, texY = 0;

    static inline Vertex make(EGE::Vec3f coords, sf::Color color = sf::Color::White, EGE::Vec2f texCoords = EGE::Vec2f())
    {
        return Vertex{coords.x, coords.y, coords.z, (char)((short)color.r - 128), (char)((short)color.g - 128), (char)((short)color.b - 128), (char)((short)color.a - 128), texCoords.x, texCoords.y};
    }
};

class Renderer
{
public:
    Renderer(sf::RenderTarget& target)
    : m_target(target) {}

    // Common renderers.
    // For more advanced shapes, use SFML sf::Drawables.
    void renderRectangle(double x, double y, double width, double height, sf::Color color, sf::Color outlineColor = sf::Color::Transparent);
    void renderText(double x, double y, sf::Font& font, sf::String str, int size = 16);
    void renderCenteredText(double x, double y, sf::Font& font, sf::String str, int size = 16);
    void renderTexturedRectangle(double x, double y, double width, double height, sf::Texture& texture, sf::IntRect textureRect = {});
    void renderPoints(const std::vector<Vertex>& points, float pointSize = 1.0);
    void renderPrimitives(const std::vector<Vertex>& points, sf::PrimitiveType type);

    // GUI theme renderers
    void renderButtonLike(double x, double y, double width, double height);
    void renderTextBoxLike(double x, double y, double width, double height);

    sf::RenderTarget& getTarget()
    {
        return m_target;
    }

    void setStates(const RenderStates& states)
    {
        m_states = states;
    }

private:
    void applyStates();

    // noncopyable, nonmoveable
    Renderer(const Renderer& other) = delete;
    Renderer(Renderer&& other) = delete;
    Renderer& operator=(const Renderer& other) = delete;
    Renderer& operator=(Renderer&& other) = delete;

    sf::RenderTarget& m_target;
    RenderStates m_states;
};

}
