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

#include "RadioButton.h"

#include <ege/gui/GUIGameLoop.h>

namespace EGE
{

void RadioButton::render(Renderer& renderer) const
{
    sf::RenderTarget& target = renderer.getTarget();

    sf::CircleShape cs;
    cs.setFillColor(sf::Color(255, 255, 255));
    cs.setOutlineThickness(1.f);

    // background
    cs.setPosition(2.f, 2.f);
    cs.setOutlineColor(sf::Color(60, 60, 60));
    cs.setRadius(5.5f);
    target.draw(cs, renderer.getStates().sfStates());

    // border
    cs.setRadius(6.f);
    cs.setPosition(1.f, 1.f);
    cs.setFillColor(sf::Color::Transparent);
    cs.setOutlineColor(sf::Color(173, 173, 173));
    target.draw(cs, renderer.getStates().sfStates());

    cs.setRadius(6.5f);
    cs.setPosition(1.f, 1.f);
    cs.setOutlineColor(sf::Color(210, 210, 210));
    target.draw(cs, renderer.getStates().sfStates());

    // border if clicked
    if(m_leftClicked)
    {
        cs.setRadius(4.5f);
        cs.setPosition(3.f, 3.f);
        cs.setOutlineColor(sf::Color(70, 70, 70));
        target.draw(cs, renderer.getStates().sfStates());
    }

    // check
    if(isChecked())
    {
        cs.setFillColor(sf::Color(60, 60, 60));
        cs.setOutlineColor(sf::Color::Transparent);
        cs.setPosition(5.f, 5.f);
        cs.setRadius(2.5f);
        target.draw(cs, renderer.getStates().sfStates());
    }

    // label
    if(!getLabel().isEmpty())
    {
        auto font = getParentWidget()->getLoop().getResourceManager()->getDefaultFont();
        ASSERT(font);
        sf::Text text(getLabel(), *font, 12);
        text.setPosition(20.f, 0.f);
        text.setFillColor(sf::Color(m_labelColor.r, m_labelColor.g, m_labelColor.b, m_labelColor.a));
        target.draw(text, renderer.getStates().sfStates());
    }

    Widget::render(renderer);
}

void RadioButton::onClick(EGE::Vec2d pos)
{
    CheckBox::onClick(pos);
    m_radioGroup.didClick(*this);
}

void RadioButton::updateLayout()
{
    Widget::updateLayout();
    auto font = getLoop().getResourceManager()->getDefaultFont();
    ASSERT(font);
    sf::Text text(getLabel(), *font, 12);
    if(getRawSize().x.unit() == EGE_LAYOUT_AUTO || getRawSize().y.unit() == EGE_LAYOUT_AUTO)
        setSize(Vec2d(text.getLocalBounds().width + 25.0, 25.0));

}

}
