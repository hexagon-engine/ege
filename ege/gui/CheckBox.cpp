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

#include "CheckBox.h"

#include <ege/gui/GUIGameLoop.h>

namespace EGE
{

void CheckBox::render(Renderer& renderer) const
{
    sf::RenderTarget& target = renderer.getTarget();

    sf::RectangleShape rs;
    rs.setFillColor(sf::Color(255, 255, 255));
    rs.setOutlineThickness(1.f);

    // background
    rs.setPosition(2.f, 2.f);
    rs.setOutlineColor(sf::Color(60, 60, 60));
    rs.setSize(sf::Vector2f(11.f, 11.f));
    target.draw(rs, renderer.getStates().sfStates());

    // border
    rs.setSize(sf::Vector2f(12.f, 12.f));
    rs.setPosition(1.f, 1.f);
    rs.setFillColor(sf::Color::Transparent);
    rs.setOutlineColor(sf::Color(173, 173, 173));
    target.draw(rs, renderer.getStates().sfStates());

    rs.setSize(sf::Vector2f(13.f, 13.f));
    rs.setPosition(1.f, 1.f);
    rs.setOutlineColor(sf::Color(210, 210, 210));
    target.draw(rs, renderer.getStates().sfStates());

    // border if clicked
    if(m_leftClicked)
    {
        rs.setSize(sf::Vector2f(9.f, 9.f));
        rs.setPosition(3.f, 3.f);
        rs.setOutlineColor(sf::Color(70, 70, 70));
        target.draw(rs, renderer.getStates().sfStates());
    }

    // check
    if(m_checked)
    {
        rs.setFillColor(sf::Color(60, 60, 60));
        rs.setOutlineColor(sf::Color::Transparent);
        rs.setPosition(5.f, 5.f);
        rs.setSize(sf::Vector2f(5.f, 5.f));
        target.draw(rs, renderer.getStates().sfStates());
    }

    // label
    auto font = getLoop().getResourceManager()->getDefaultFont();
    ASSERT(font);
    sf::Text text(getLabel(), *font, 12);
    text.setPosition(20.f, 0.f);
    text.setFillColor(sf::Color(m_labelColor.r * 255, m_labelColor.g * 255, m_labelColor.b * 255, m_labelColor.a * 255));
    target.draw(text, renderer.getStates().sfStates());

    Widget::render(renderer);
}

void CheckBox::updateLayout()
{
    Widget::updateLayout();

    // label (generate)
    auto font = getLoop().getResourceManager()->getDefaultFont();
    ASSERT(font);
    sf::Text text(getLabel(), *font, 12);

    if(getRawSize().x.unit() == EGE_LAYOUT_AUTO || getRawSize().y.unit() == EGE_LAYOUT_AUTO)
        setSize(LVec2d({text.getLocalBounds().width + 25.f, EGE_LAYOUT_PIXELS}, {25.f, EGE_LAYOUT_PIXELS}));
}

void CheckBox::onClick(EGE::Vec2d position)
{
    Button::onClick(position);
    m_checked = !m_checked;
}

}
