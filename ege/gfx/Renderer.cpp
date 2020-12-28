/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#include "Renderer.h"

#include <SFML/OpenGL.hpp>

namespace EGE
{

void Renderer::renderRectangle(double x, double y, double width, double height, sf::Color color, sf::Color outlineColor)
{
    sf::RectangleShape rs(sf::Vector2f(width, height));
    rs.setPosition(x, y);
    rs.setFillColor(color);
    if(outlineColor != sf::Color::Transparent)
    {
        rs.setOutlineColor(outlineColor);
        rs.setOutlineThickness(1.f);
    }
    getTarget().draw(rs, m_states.sfStates());
}

void Renderer::renderText(double x, double y, sf::Font& font, sf::String str, int size, sf::Color color, float scale)
{
    sf::Text text(str, font, size * scale);
    text.setPosition(x, y);
    text.setFillColor(color);
    text.setScale(1.f / scale, 1.f / scale);
    getTarget().draw(text, m_states.sfStates());
}

void Renderer::renderTextWithBackground(double x, double y, sf::Font& font, sf::String str, Renderer::TextWithBackgroundSettings settings)
{
    sf::Text text(str, font, settings.font_size);
    text.setPosition(x, y);

    switch(settings.text_align)
    {
    case Renderer::TextAlign::Left:
        break;
    case Renderer::TextAlign::Center:
        text.setOrigin(text.getLocalBounds().getSize() / 2.f);
        break;
    case Renderer::TextAlign::Right:
        text.setOrigin(text.getLocalBounds().getSize());
        break;
    default:
        CRASH();
    }

    auto textRect = text.getGlobalBounds();
    textRect.height += 5.f * settings.font_size / 15.f; //SFML text bounds bug??
    textRect.width += 1.f * settings.font_size / 15.f;

    // Background
    renderRectangle(textRect.left - settings.padding,
                    textRect.top - settings.padding,
                    textRect.width + settings.padding * 2,
                    textRect.height + settings.padding * 2,
                    settings.background_color);

    // Text
    text.setFillColor(settings.color);
    getTarget().draw(text, m_states.sfStates());
}

void Renderer::renderCenteredText(double x, double y, sf::Font& font, sf::String str, int size)
{
    sf::Text text(str, font, size);
    text.setPosition(x, y);
    text.setOrigin(text.getLocalBounds().getSize() / 2.f);
    text.setFillColor(sf::Color::Black);
    getTarget().draw(text, m_states.sfStates());
}

void Renderer::renderTexturedRectangle(double x, double y, double width, double height, sf::Texture& texture, sf::IntRect textureRect)
{
    if(textureRect == sf::IntRect())
        textureRect = sf::IntRect(0, 0, texture.getSize().x, texture.getSize().y);
    sf::RectangleShape rs(sf::Vector2f(width, height));
    rs.setPosition(x, y);
    rs.setTexture(&texture);
    rs.setTextureRect(textureRect);
    getTarget().draw(rs, m_states.sfStates());
}

void Renderer::renderPoints(const std::vector<Vertex>& points, float pointSize)
{
    NOT_IMPLEMENTED("Use renderPrimitives()");
    if(points.empty())
        return;

    //getTarget().pushGLStates();

    applyStates();

    // TODO: Maybe make it Renderer-local?
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    glVertexPointer(3, GL_FLOAT, sizeof(EGE::Vertex), (char*)points.data());
    glColorPointer(4, GL_BYTE, sizeof(EGE::Vertex), (char*)points.data() + sizeof(float) * 3);
    glTexCoordPointer(2, GL_FLOAT, sizeof(EGE::Vertex), (char*)points.data() + sizeof(float) * 3 + sizeof(char) * 4);

    glPointSize(pointSize);
    glDrawArrays(GL_POINTS, 0, points.size());

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);

    //getTarget().resetGLStates();

    //getTarget().popGLStates();
}

void Renderer::renderPrimitives(const std::vector<Vertex>& points, sf::PrimitiveType type)
{
    sf::VertexArray varr(type, points.size());
    Size _size = points.size();
    for(Size s = 0; s < _size; s++)
    {
        const Vertex& vertex = points[s];
        varr[s] = (sf::Vertex(sf::Vector2f(vertex.x, vertex.y),
                    sf::Color((int)vertex.r + 128, (int)vertex.g + 128, (int)vertex.b + 128, (int)vertex.a + 128),
                    sf::Vector2f(vertex.texX, vertex.texY)));
    }
    getTarget().draw(varr, m_states.sfStates());
}

void Renderer::renderButtonLike(double x, double y, double width, double height)
{
    sf::RectangleShape rs;
    rs.setOutlineThickness(1.f);

    rs.setFillColor(sf::Color(209, 209, 209));
    rs.setSize(sf::Vector2f(width - 2.f, height - 2.f));
    rs.setPosition(sf::Vector2f(x + 1.f, y + 1.f));
    rs.setOutlineColor(sf::Color(255, 255, 255));
    getTarget().draw(rs, m_states.sfStates());

    rs.setFillColor(sf::Color::Transparent);
    rs.setSize(sf::Vector2f(width - 1.f, height - 1.f));
    rs.setPosition(sf::Vector2f(x, y));
    rs.setOutlineColor(sf::Color(60, 60, 60));
    getTarget().draw(rs, m_states.sfStates());
}

void Renderer::renderTextBoxLike(double x, double y, double width, double height)
{
    sf::RectangleShape rs;
    rs.setFillColor(sf::Color(255, 255, 255));
    rs.setOutlineThickness(1.f);

    // background
    rs.setPosition(x + 2.f, y + 2.f);
    rs.setOutlineColor(sf::Color(60, 60, 60));
    rs.setSize(sf::Vector2f(width, height) - sf::Vector2f(4.f, 4.f));
    getTarget().draw(rs);

    // border
    rs.setSize(sf::Vector2f(width, height) - sf::Vector2f(3.f, 3.f));
    rs.setPosition(x + 1.f, y + 1.f);
    rs.setFillColor(sf::Color::Transparent);
    rs.setOutlineColor(sf::Color(173, 173, 173));
    getTarget().draw(rs);

    rs.setSize(sf::Vector2f(width, height) - sf::Vector2f(2.f, 2.f));
    rs.setPosition(x + 1.f, y + 1.f);
    rs.setOutlineColor(sf::Color(210, 210, 210));
    getTarget().draw(rs);
}

void Renderer::applyStates()
{
    glEnable(GL_TEXTURE_2D);
    sf::Texture::bind(m_states.sfStates().texture, sf::Texture::Pixels);
    sf::Shader::bind(m_states.sfStates().shader);

    // [SFML] Apply view

    // Set the viewport
    sf::IntRect viewport = getTarget().getViewport(getTarget().getView());
    int top = getTarget().getSize().y - (viewport.top + viewport.height);
    glViewport(viewport.left, top, viewport.width, viewport.height);

    // Set the projection matrix
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(getTarget().getView().getTransform().getMatrix());

    // Go back to model-view mode
    glMatrixMode(GL_MODELVIEW);
}

}
