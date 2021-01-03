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
    EGE::Vec2d tmp1(event.x - getPosition().x, event.y - getPosition().y);
    auto bounds = getKnobBounds();
    m_dragPos = tmp1 - EGE::Vec2d(bounds.getPosition().x, bounds.getPosition().y);
    m_dragValue = m_value;
    if(getKnobBounds().contains(sf::Vector2f(tmp1.x, tmp1.y)))
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
        rel -= getPosition();

        // save value relatively to sb type and starting drag pos
        double rv = 0.0;
        switch(m_type)
        {
            case Type::Horizontal: rv = rel.x - m_dragPos.x; break;
            case Type::Vertical: rv = rel.y - m_dragPos.y; break;
            default: CRASH(); break;
        }

        // calculate new value
        double newVal = (rv - 20.0) / (m_length - 55.0);

        // clamp value and scale by max value
        double nvClamp = std::min(1.0, std::max(0.0, newVal)) * m_maxValue;

        // set new value and call callback (if event position is positive)
        if(m_updateCallback && nvClamp != m_value)
            m_updateCallback(nvClamp);

        m_value = nvClamp;
    }
}

void ScrollBar::updateGeometry(Renderer& renderer)
{
    switch(m_type)
    {
        case Type::Horizontal: setSize(Vec2d(m_length, 20.0)); break;
        case Type::Vertical: setSize(Vec2d(20.0, m_length)); break;
        default: CRASH(); break;
    }

    // Layouting
    runLayoutUpdate();
}

sf::FloatRect ScrollBar::getKnobBounds() const
{
    sf::FloatRect rect(0.f, 0.f, 20.f, 20.f);
    double knobPos = (m_value / m_maxValue) * (m_length - 55.0) + 20.0;

    switch(m_type)
    {
        case Type::Horizontal: rect.left = knobPos; rect.width = 15.f; break;
        case Type::Vertical: rect.top = knobPos; rect.height = 15.f; break;
        default: CRASH(); break;
    }

    return rect;
}

void ScrollBar::render(Renderer& renderer) const
{
    // background
    renderer.renderRectangle(0.0, 20.0, getSize().x, getSize().y - 40.0, sf::Color(175, 175, 175));

    // scroll buttons
    renderer.renderButtonLike(0.0, 0.0, 20.0, 20.0);
    renderer.renderButtonLike(0.0, getSize().y - 20.0, 20.0, 20.0);

    // knob
    sf::FloatRect rect = getKnobBounds();
    renderer.renderButtonLike(rect.left, rect.top, rect.width, rect.height);

    Widget::render(renderer);
}

}
