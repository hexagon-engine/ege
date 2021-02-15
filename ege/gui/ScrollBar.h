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

#pragma once

#include "Widget.h"

#include <ege/gfx/RenderStates.h>
#include <SFML/Graphics.hpp>

namespace EGE
{

class ScrollBar : public Widget
{
public:
    enum Type
    {
        Horizontal,
        Vertical
    };

    explicit ScrollBar(Widget& parent, String id = "ScrollBar")
    : Widget(parent, id) {}

    double getValue() const { return m_value; }
    double getMaxValue() const { return m_maxValue; }

    // Default max value is 1.0.
    void setValue(double value)
    {
        ASSERT(value >= 0.0);
        ASSERT(value < m_maxValue);
        m_value = value;
        setGeometryNeedUpdate();
    }

    // 1.0 - one page.
    void setMaxValue(double value)
    {
        if(value < 1)
        {
            value = 1;
            // TODO: disable widget
        }
        m_maxValue = value;
        if(m_value > m_maxValue - 1)
            scroll(m_maxValue - 1);
        setGeometryNeedUpdate();
    }

    virtual void onMouseButtonPress(sf::Event::MouseButtonEvent& event);
    virtual void onMouseButtonRelease(sf::Event::MouseButtonEvent& event);
    virtual void onMouseMove(sf::Event::MouseMoveEvent& event);

    void setUpdateCallback(std::function<void(double)> func)
    {
        m_updateCallback = func;

        // Call the handler.
        scroll(0);
    }

    void setType(Type type)
    {
        m_type = type;
        setGeometryNeedUpdate();
    }

    void setLength(LayoutSizeD length)
    {
        m_length = length;
        setGeometryNeedUpdate();
    }

    void scroll(double page);
    void scrollWithMouse(Vec2d mousePos);
    void scrollToPosition(double pos);

    double getScrollableSpace() const;

protected:
    virtual void render(Renderer& renderer) const override;

    bool m_knobMouseOver = false;
    bool m_knobDragged = false;

    sf::FloatRect getKnobBounds() const;
    virtual void updateLayout() override;

    std::function<void(double)> m_updateCallback;
    double m_value = 0.0;
    double m_maxValue = 1.0;
    LayoutSizeD m_length = 0.0;
    EGE::Vec2d m_dragPos;
    double m_dragValue = 0.0;
    Type m_type;
};

}
