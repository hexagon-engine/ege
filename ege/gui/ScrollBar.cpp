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

#include "ScrollBar.h"

#include <ege/gfx/Renderer.h>
#include <ege/gui/GUIGameLoop.h>

namespace EGE
{

void ScrollBar::onMouseButtonPress(sf::Event::MouseButtonEvent& event)
{
    Widget::onMouseButtonPress(event);
    EGE::Vec2d relative(event.x - getAbsolutePosition().x, event.y - getAbsolutePosition().y);
    auto bounds = getKnobBounds();
    m_dragPos = relative - EGE::Vec2d(bounds.getPosition().x, bounds.getPosition().y);
    m_dragValue = m_value;
    if(getKnobBounds().contains(sf::Vector2f(relative.x, relative.y)))
        m_knobDragged = true;
}

void ScrollBar::onMouseButtonRelease(sf::Event::MouseButtonEvent& event)
{
    Widget::onMouseButtonRelease(event);
    m_knobDragged = false;
}

void ScrollBar::onMouseMove(sf::Event::MouseMoveEvent& event)
{
    Widget::onMouseMove(event);
    if(m_knobDragged)
    {
        // calculate relative mouse position
        EGE::Vec2d rel(event.x, event.y);
        rel -= getAbsolutePosition();

        // calculate new value
        double newVal;

        switch(m_type)
        {
            case Type::Horizontal: newVal = (rel.x - m_dragPos.x - 20.0) / (getSize().x - 55.0); break;
            case Type::Vertical: newVal = (rel.y - m_dragPos.y - 20.0) / (getSize().y - 55.0); break;
            default: CRASH(); break;
        }

        // clamp value and scale by max value
        double nvClamp = std::min(1.0, std::max(0.0, newVal)) * m_maxValue;

        // set new value and call callback (if event position is positive)
        if(m_updateCallback && nvClamp != m_value)
            m_updateCallback(nvClamp);

        m_value = nvClamp;
    }
}

void ScrollBar::updateLayout()
{
    Widget::updateLayout();
    switch(m_type)
    {
        case Type::Horizontal: setSize(LVec2d(m_length, 20.0)); break;
        case Type::Vertical: setSize(LVec2d(20.0, m_length)); break;
        default: CRASH(); break;
    }

}

sf::FloatRect ScrollBar::getKnobBounds() const
{
    sf::FloatRect rect(0.f, 0.f, 20.f, 20.f);

    switch(m_type)
    {
        case Type::Horizontal: rect.left = (m_value / m_maxValue) * (getSize().x - 55.0) + 20.0; rect.width = 15.f; break;
        case Type::Vertical: rect.top = (m_value / m_maxValue) * (getSize().y - 55.0) + 20.0; rect.height = 15.f; break;
        default: CRASH(); break;
    }

    return rect;
}

void ScrollBar::render(Renderer& renderer) const
{
    // background
    renderer.renderRectangle(0.0, 20.0, getSize().x, getSize().y - 40.0, ColorRGBA::fromBytes(175, 175, 175));

    // scroll buttons
    renderer.renderButtonLike(0.0, 0.0, 20.0, 20.0);
    renderer.renderButtonLike(0.0, getSize().y - 20.0, 20.0, 20.0);

    // knob
    sf::FloatRect rect = getKnobBounds();
    renderer.renderButtonLike(rect.left, rect.top, rect.width, rect.height);

    Widget::render(renderer);
}

}
