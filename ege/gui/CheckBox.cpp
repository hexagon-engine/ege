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
#include <ege/util/Color.h>

namespace EGE
{

void CheckBox::render(Renderer& renderer) const
{
    sf::RenderTarget& target = renderer.getTarget();

    double center = getSize().y / 2;

    // background
    renderer.getThemeRenderer()->renderTextBoxLikeBackground(renderer, 0, center - 7.5, 15, 15);
    renderer.getThemeRenderer()->renderTextBoxLikeBorder(renderer, 0, center - 7.5, 15, 15);

    // border if clicked
    if(m_leftClicked)
        renderer.renderRectangle(2, center - 5, 11, 11, ColorRGBA::fromBytes(210, 210, 210));

    // check
    if(m_checked)
        renderer.renderRectangle(5, center - 2.5, 5, 5, ColorRGBA::fromBytes(60, 60, 60));

    // label
    if(!getLabel().isEmpty())
    {
        auto font = getLoop().getResourceManager()->getDefaultFont();
        ASSERT(font);
        sf::Text text(getLabel(), *font, 12);
        double centerAlign = center - text.getGlobalBounds().height / 2;
        text.setPosition(20.f, centerAlign - 1);
        text.setFillColor(sf::Color(m_labelColor.r, m_labelColor.g, m_labelColor.b, m_labelColor.a));
        target.draw(text, renderer.getStates().sfStates());
    }

    Widget::render(renderer);
}

void CheckBox::updateLayout()
{
    Widget::updateLayout();

    // label (generate)
    if(!getLabel().isEmpty())
    {
        auto font = getLoop().getResourceManager()->getDefaultFont();
        ASSERT(font);
        sf::Text text(getLabel(), *font, 12);

        if(getRawSize().x.unit() == EGE_LAYOUT_AUTO || getRawSize().y.unit() == EGE_LAYOUT_AUTO)
            setSize(LVec2d({text.getLocalBounds().width + 25.f, EGE_LAYOUT_PIXELS}, {25.f, EGE_LAYOUT_PIXELS}));
    }
}

void CheckBox::onClick(EGE::Vec2d position)
{
    Button::onClick(position);
    m_checked = !m_checked;
    fire<CheckEvent>(m_checked);
}

}
