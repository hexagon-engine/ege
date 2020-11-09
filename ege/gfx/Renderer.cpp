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
    text.setFillColor(sf::Color::Black);
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

    //getTarget().resetGLStates();

    //getTarget().popGLStates();
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
