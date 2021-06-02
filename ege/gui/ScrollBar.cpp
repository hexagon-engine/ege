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

#include <ege/debug/Logger.h>
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
        if(m_maxValue != 1)
        {
            Vec2d rel(event.x, event.y);
            scrollWithMouse(rel);
        }
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
        case Type::Horizontal:
            rect.width = std::max(15.0, (1 / m_maxValue) * (getSize().x - 40));
            rect.left = (m_value / m_maxValue) * (getSize().x - 40) + 20.0;
            break;
        case Type::Vertical:
            rect.height = std::max(15.0, (1 / m_maxValue) * (getSize().y - 40));
            rect.top = (m_value / m_maxValue) * (getSize().y - 40) + 20.0;
            break;
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

void ScrollBar::scrollWithMouse(Vec2d mousePos)
{
    mousePos -= getAbsolutePosition();

    // calculate new value
    double newVal;

    auto knobBounds = getKnobBounds();

    switch(m_type)
    {
        case Type::Horizontal: newVal = (mousePos.x - m_dragPos.x - 20.0) / (getSize().x - 40 - knobBounds.width); break;
        case Type::Vertical: newVal = (mousePos.y - m_dragPos.y - 20.0) / (getSize().y - 40 - knobBounds.height); break;
        default: CRASH(); break;
    }

    // clamp value and scale by max value
    double nvClamp = std::min(1.0, std::max(0.0, newVal)) * (m_maxValue - 1);

    // set new value and call callback (if event position is positive)
    scroll(nvClamp);
}

void ScrollBar::scroll(double val)
{
    val = std::min(m_maxValue - 1, std::max(0.0, val));
    if(m_updateCallback && val != m_value)
        m_updateCallback(val);

    m_value = val;
    ege_log.debug() << "scroll(" << val << ")";
}

void ScrollBar::scrollToPosition(double pos)
{
    double space = getScrollableSpace();
    ege_log.debug() << "ScrollBar: " << pos << "/" << space << " -> " << pos / space << " (max=" << getMaxValue() << ")";
    scroll(pos / space * getMaxValue());
}

double ScrollBar::getScrollableSpace() const
{
    switch(m_type)
    {
        case Type::Horizontal: return getSize().x * getMaxValue();
        case Type::Vertical: return getSize().y * getMaxValue();
    }
    return 0.0;
}

double ScrollBar::getScrollPosition() const
{
    switch(m_type)
    {
        case Type::Horizontal: return getSize().x * m_value;
        case Type::Vertical: return getSize().y * m_value;
    }
    return 0.0;
}

}
