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

#include "Slider.h"

#include <cmath>

namespace EGE
{

void Slider::scrollWithMouse(double x)
{
    double realSize = getSize().x - 10;
    x *= realSize / getSize().x;
    x += 5;
    m_value = x * m_maxValue / realSize;
    if(m_value < 0)
        m_value = 0;
    if(m_value > m_maxValue)
        m_value = m_maxValue;

    // Apply step.
    m_value /= m_step;
    m_value = round(m_value);
    m_value *= m_step;

    ege_log.debug() << m_value;
}

void Slider::onMouseButtonPress(sf::Event::MouseButtonEvent& event)
{
    Widget::onMouseButtonPress(event);
    scrollWithMouse(event.x - getAbsolutePosition().x);
    if(event.button == sf::Mouse::Left && m_mouseOver)
        m_knobDragged = true;
}

void Slider::onMouseButtonRelease(sf::Event::MouseButtonEvent& event)
{
    if(event.button == sf::Mouse::Left)
        m_knobDragged = false;
}

void Slider::onMouseMove(sf::Event::MouseMoveEvent& event)
{
    Widget::onMouseMove(event);
    if(m_knobDragged)
    {
        ege_log.debug() << "Slider slide.";
        scrollWithMouse(event.x - getAbsolutePosition().x);
    }
}

void Slider::render(Renderer& renderer) const
{
    renderer.renderRectangle(0, getSize().y / 2 - 3, getSize().x, 6, ColorRGBA::fromBytes(200, 200, 200));
    auto knob = getKnobBounds();
    renderer.renderButtonLike(knob.position.x, knob.position.y, knob.size.x, knob.size.y);
}

RectF Slider::getKnobBounds() const
{
    double knobSize = 8.0;
    return RectF(m_value * (getSize().x - 10) + 5 - knobSize / 2, 0, knobSize, getSize().y);
}

}

