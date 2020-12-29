/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
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

    explicit ScrollBar(Widget& parent)
    : Widget(parent) {}

    double getValue() const
    {
        return m_value;
    }

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
        ASSERT(value > 0.0);
        m_maxValue = value;
        setGeometryNeedUpdate();
    }

    virtual void onMouseButtonPress(sf::Event::MouseButtonEvent& event);
    virtual void onMouseButtonRelease(sf::Event::MouseButtonEvent& event);
    virtual void onMouseMove(sf::Event::MouseMoveEvent& event);

    void setUpdateCallback(std::function<void(double)> func)
    {
        m_updateCallback = func;
    }

    void setType(Type type)
    {
        m_type = type;
        setGeometryNeedUpdate();
    }

    void setLength(double length)
    {
        ASSERT(length >= 0.0);
        m_length = length;
        setGeometryNeedUpdate();
    }

protected:
    virtual void render(Renderer& renderer) const override;

    bool m_knobMouseOver = false;
    bool m_knobDragged = false;

    sf::FloatRect getKnobBounds() const;
    virtual void updateGeometry(Renderer& renderer) override;

    std::function<void(double)> m_updateCallback;
    double m_value = 0.0;
    double m_maxValue = 1.0;
    double m_length = 0.0;
    EGE::Vec2d m_dragPos;
    double m_dragValue = 0.0;
    Type m_type;
};

}
