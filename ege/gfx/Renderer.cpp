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

}
