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
// TODO: use some uninitialized vector type in renderer
struct Vertex
{
    float x, y, z;
    char r, g, b, a;
    float texX, texY;

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
    // For more complex shapes, use SFML sf::Drawables.
    void renderRectangle(double x, double y, double width, double height, sf::Color color, sf::Color outlineColor = sf::Color::Transparent);

    // scale - resolution scale, used when rendering small text with big resolution
    void renderText(double x, double y, sf::Font& font, sf::String str, int size = 16, sf::Color color = sf::Color::White, float scale = 1.f);

    enum TextAlign
    {
        Left,
        Center,
        Right
    };

    struct TextWithBackgroundSettings
    {
        sf::Color background_color = sf::Color::Black;
        sf::Color color = sf::Color::White;
        int font_size = 16;
        double padding = 5.0;
        TextAlign text_align = Left;

        TextWithBackgroundSettings() {};
    };

    void renderTextWithBackground(double x, double y, sf::Font& font, sf::String str, TextWithBackgroundSettings settings = {});
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
