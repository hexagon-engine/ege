/*
*
*   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*
*      ,----  ,----  ,----
*      |      |      |
*      |----  | --,  |----
*      |      |   |  |
*      '----  '---'  '----
*
*     Framework Library for Hexagon
*
*    Copyright (c) Sppmacd 2020 - 2021
*
*    Permission is hereby granted, free of charge, to any person obtaining a copy
*    of this software and associated documentation files (the "Software"), to deal
*    in the Software without restriction, including without limitation the rights
*    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
*    copies of the Software, and to permit persons to whom the Software is
*    furnished to do so, subject to the following conditions:
*
*    The above copyright notice and this permission notice shall be included in all
*    copies or substantial portions of the Software.
*
*    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
*    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
*    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
*    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
*    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
*    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
*    SOFTWARE.
*
*   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*
*/

#pragma once

#include "DefaultThemeRenderer.h"
#include "RenderStates.h"

#include <ege/util/Vector.h>
#include <ege/util/Types.h>
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
    : m_target(target) { setThemeRenderer(std::make_unique<DefaultThemeRenderer>()); }

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

    sf::RenderTarget& getTarget() { return m_target; }
    void setStates(const RenderStates& states) { m_states = states; }
    const RenderStates& getStates() { return m_states; }

    void setThemeRenderer(UniquePtr<ThemeRenderer> themeRenderer) { m_themeRenderer.swap(themeRenderer); }
    ThemeRenderer* getThemeRenderer() { return m_themeRenderer.get(); }

    virtual void renderButtonLike(double x, double y, double width, double height) { m_themeRenderer->renderButtonLike(*this, x, y, width, height); }
    virtual void renderTextBoxLikeBorder(double x, double y, double width, double height) { m_themeRenderer->renderTextBoxLikeBorder(*this, x, y, width, height); }
    virtual void renderTextBoxLikeBackground(double x, double y, double width, double height) { m_themeRenderer->renderTextBoxLikeBackground(*this, x, y, width, height); }

private:
    void applyStates();

    // noncopyable, nonmoveable
    Renderer(const Renderer& other) = delete;
    Renderer(Renderer&& other) = delete;
    Renderer& operator=(const Renderer& other) = delete;
    Renderer& operator=(Renderer&& other) = delete;

    sf::RenderTarget& m_target;
    RenderStates m_states;
    UniquePtr<ThemeRenderer> m_themeRenderer;
};

}
