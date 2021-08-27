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

#include "Button.h"

#include <ege/gui/GUIGameLoop.h>

namespace EGE
{

void Button::onMouseButtonRelease(sf::Event::MouseButtonEvent& event)
{
    handleClick(EGE::Vec2d(event.x, event.y));
    Widget::onMouseButtonRelease(event); //it removes m_leftClicked state
}

void Button::onTouchEnd(sf::Event::TouchEvent& event)
{
    handleClick(EGE::Vec2d(event.x, event.y));
    Widget::onTouchEnd(event); //it removes m_leftClicked state
}

void Button::render(Renderer& renderer) const
{
    sf::RenderTarget& target = renderer.getTarget();
    auto size = getSize();

    // base
    if(m_leftClicked)
    {
        sf::RectangleShape rs;
        rs.setOutlineThickness(1.f);

        rs.setFillColor(sf::Color(192, 192, 192));
        rs.setSize(sf::Vector2f(size.x, size.y) - sf::Vector2f(2.f, 2.f));
        rs.setPosition(2.f, 2.f);
        rs.setOutlineColor(sf::Color(29, 29, 29));
        target.draw(rs, renderer.getStates().sfStates());

        rs.setFillColor(sf::Color::Transparent);
        rs.setSize(sf::Vector2f(size.x, size.y) - sf::Vector2f(1.f, 1.f));
        rs.setPosition(1.f, 1.f);
        rs.setOutlineColor(sf::Color(200, 200, 200));
        target.draw(rs, renderer.getStates().sfStates());

        rs.setSize(sf::Vector2f(size.x, size.y) - sf::Vector2f(1.f, 1.f));
        rs.setPosition(0.f, 0.f);
        rs.setOutlineColor(sf::Color(255, 255, 255));
        target.draw(rs, renderer.getStates().sfStates());
    }
    else if(m_mouseOver)
    {
        sf::RectangleShape rs;
        rs.setOutlineThickness(1.f);

        rs.setFillColor(sf::Color(210, 210, 210));
        rs.setSize(sf::Vector2f(size.x, size.y) - sf::Vector2f(2.f, 2.f));
        rs.setPosition(1.f, 1.f);
        rs.setOutlineColor(sf::Color(255, 255, 255));
        target.draw(rs, renderer.getStates().sfStates());

        rs.setFillColor(sf::Color::Transparent);
        rs.setSize(sf::Vector2f(size.x, size.y) - sf::Vector2f(1.f, 1.f));
        rs.setPosition(0.f, 0.f);
        rs.setOutlineColor(sf::Color(29, 29, 29));
        target.draw(rs, renderer.getStates().sfStates());
    }
    else
    {
        sf::RectangleShape rs;
        rs.setOutlineThickness(1.f);

        rs.setFillColor(sf::Color(200, 200, 200));
        rs.setSize(sf::Vector2f(size.x, size.y) - sf::Vector2f(2.f, 2.f));
        rs.setPosition(1.f, 1.f);
        rs.setOutlineColor(sf::Color(255, 255, 255));
        target.draw(rs, renderer.getStates().sfStates());

        rs.setFillColor(sf::Color::Transparent);
        rs.setSize(sf::Vector2f(size.x, size.y) - sf::Vector2f(1.f, 1.f));
        rs.setPosition(0.f, 0.f);
        rs.setOutlineColor(sf::Color(29, 29, 29));
        target.draw(rs, renderer.getStates().sfStates());
    }

    // image
    double labelPosition = 10;
    if(m_texture)
    {
        renderer.renderTexturedRectangle(4, 4, getSize().y - 8, getSize().y - 8, m_texture->getTexture());
        labelPosition += getSize().y - 4;
    }

    // label
    if(!m_label.isEmpty())
    {
        auto font = getParent<Widget>()->getLoop().getResourceManager()->getDefaultFont();
        ASSERT(font);
        sf::Text text(m_label, *font, 12);
        text.setPosition(labelPosition, round(size.y / 2));
        text.setOrigin(0, round(text.getLocalBounds().height / 2));
        text.setFillColor(sf::Color(m_labelColor.r, m_labelColor.g, m_labelColor.b, m_labelColor.a));
        target.draw(text, renderer.getStates().sfStates());
    }

    Widget::render(renderer);
}

void Button::handleClick(EGE::Vec2d position)
{
    if(m_leftClicked)
    {
        onClick(position);
        fire<ClickEvent>();
    }
}

void Button::updateGeometry(Renderer&)
{
    if(!m_textureName.empty())
        m_texture = getResourceManager()->getTexture(m_textureName);
}

}
