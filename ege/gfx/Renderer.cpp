/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#include "Renderer.h"

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
    getTarget().draw(rs);
}

void Renderer::renderText(double x, double y, sf::Font& font, sf::String str, int size)
{
    sf::Text text(str, font, size);
    text.setPosition(x, y);
    getTarget().draw(text);
}

void Renderer::renderCenteredText(double x, double y, sf::Font& font, sf::String str, int size)
{
    sf::Text text(str, font, size);
    text.setPosition(x, y);
    text.setOrigin(text.getLocalBounds().getSize() / 2.f);
    getTarget().draw(text);
}

void Renderer::renderTexturedRectangle(double x, double y, double width, double height, sf::Texture& texture)
{
    sf::RectangleShape rs(sf::Vector2f(width, height));
    rs.setPosition(x, y);
    rs.setTexture(&texture);
    getTarget().draw(rs);
}

}
