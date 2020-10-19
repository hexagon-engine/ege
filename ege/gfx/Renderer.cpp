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

void Renderer::renderText(double x, double y, sf::Font& font, sf::String str, int size)
{
    sf::Text text(str, font, size);
    text.setPosition(x, y);
    getTarget().draw(text, m_states.sfStates());
}

void Renderer::renderCenteredText(double x, double y, sf::Font& font, sf::String str, int size)
{
    sf::Text text(str, font, size);
    text.setPosition(x, y);
    text.setOrigin(text.getLocalBounds().getSize() / 2.f);
    getTarget().draw(text, m_states.sfStates());
}

void Renderer::renderTexturedRectangle(double x, double y, double width, double height, sf::Texture& texture)
{
    sf::RectangleShape rs(sf::Vector2f(width, height));
    rs.setPosition(x, y);
    rs.setTexture(&texture);
    getTarget().draw(rs, m_states.sfStates());
}

void Renderer::renderPoints(const std::vector<Vertex>& points, float pointSize)
{
    if(points.empty())
        return;

    getTarget().pushGLStates();

    // TODO: Maybe make it Renderer-local?
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    glVertexPointer(3, GL_DOUBLE, sizeof(EGE::Vertex), (char*)points.data());
    glColorPointer(4, GL_BYTE, sizeof(EGE::Vertex), (char*)points.data() + sizeof(double) * 3);
    glTexCoordPointer(2, GL_DOUBLE, sizeof(Vertex), (char*)points.data() + sizeof(double) * 3 + sizeof(char) * 4);

    glPointSize(pointSize);
    glDrawArrays(GL_POINTS, 0, points.size());

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);

    getTarget().popGLStates();
}

void Renderer::renderPrimitives(const std::vector<Vertex>& points, sf::PrimitiveType type)
{
    if(points.empty())
        return;

    getTarget().pushGLStates();

    // TODO: Maybe make it Renderer-local?
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    glVertexPointer(3, GL_DOUBLE, sizeof(EGE::Vertex), (char*)points.data());
    glColorPointer(4, GL_BYTE, sizeof(EGE::Vertex), (char*)points.data() + sizeof(double) * 3);
    glTexCoordPointer(2, GL_DOUBLE, sizeof(Vertex), (char*)points.data() + sizeof(double) * 3 + sizeof(char) * 4);

    GLenum ptype;
    switch(type)
    {
        case sf::Points: ptype = GL_POINTS; break;
        case sf::Lines: ptype = GL_LINES; break;
        case sf::LineStrip: ptype = GL_LINE_STRIP; break;
        case sf::Triangles: ptype = GL_TRIANGLES; break;
        case sf::TriangleStrip: ptype = GL_TRIANGLE_STRIP; break;
        case sf::TriangleFan: ptype = GL_TRIANGLE_FAN; break;
        case sf::Quads: ptype = GL_QUADS; break;
        default: CRASH();
    }

    glDrawArrays(ptype, 0, points.size());

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);

    getTarget().popGLStates();
}

}
