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

#include "Label.h"
#include "GUIGameLoop.h"

#include <ege/debug/Logger.h>

namespace EGE
{

void Label::setString(sf::String str)
{
    log(LogLevel::Debug) << "Label::setString(" << str.toAnsiString() << ")";
    m_string = str;
    setGeometryNeedUpdate();
}

void Label::setTextAlign(Align align)
{
    m_align = align;
    setGeometryNeedUpdate();
}

void Label::setFontSize(int size)
{
    m_fontSize = size;
    setGeometryNeedUpdate();
}

void Label::updateGeometry(Renderer& renderer)
{
    Widget::updateGeometry(renderer);

    Vec2d position;

    if(!m_font)
        m_font = getLoop().getResourceManager()->getDefaultFont();

    m_text.setString(m_string);
    m_text.setFont(*m_font);
    m_text.setCharacterSize(m_fontSize);


    sf::FloatRect bounds = m_text.getLocalBounds();
    bounds.height += 5.f * m_fontSize / 20.f; //SFML text bounds bug??
    bounds.width += 1.f * m_fontSize / 15.f;
    switch(m_align)
    {
        case Align::Left:
            m_text.setOrigin(0.f, 0.f);
            position = Vec2d();
            break;
        case Align::Center:
            m_text.setOrigin(bounds.width / 2.0, bounds.height / 2.0);
            position = getSize() / 2.0;
            break;
        case Align::Right:
            m_text.setOrigin(bounds.width, bounds.height);
            position = getSize();
            break;
    }

    m_text.setPosition(position.x, position.y);
    m_text.setFillColor(m_color);
}

void Label::updateLayout()
{
    Widget::updateLayout();

    sf::FloatRect bounds = m_text.getLocalBounds();
    bounds.height += 5.f * m_fontSize / 20.f; //SFML text bounds bug??
    bounds.width += 1.f * m_fontSize / 15.f;

    if(getRawSize().x.unit() == EGE_LAYOUT_AUTO)
    {
        // Set to text bounds.
        setSize({bounds.width, getRawSize().y});
    }
    if(getRawSize().y.unit() == EGE_LAYOUT_AUTO)
    {
        // Set to text bounds.
        setSize({getRawSize().x, bounds.height});
    }
}

void Label::render(Renderer& renderer) const
{
    renderer.getTarget().draw(m_text);
    Widget::render(renderer);
}

}
