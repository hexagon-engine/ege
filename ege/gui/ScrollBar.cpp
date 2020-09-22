/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#include "ScrollBar.h"

#include <ege/gui/GUIGameLoop.h>

namespace EGE
{

void ScrollBar::onMouseButtonPress(sf::Event::MouseButtonEvent& event)
{
    Widget::onMouseButtonPress(event);
    sf::Vector2f tmp1 = sf::Vector2f(event.x - getPosition().x, event.y - getPosition().y);
    m_dragPos = tmp1 - getKnobBounds().getPosition();
    m_dragValue = m_value;
    if(getKnobBounds().contains(tmp1))
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
        sf::Vector2f rel(event.x, event.y);
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
        double newVal = (rv - 20.0) / (m_length - 60.0);

        // clamp value and scale by max value
        double nvClamp = std::min(1.0, std::max(0.0, newVal)) * m_maxValue;

        // set new value and call callback (if event position is positive)
        if(m_updateCallback && nvClamp != m_value)
            m_updateCallback(nvClamp);

        m_value = nvClamp;
    }
}

void ScrollBar::render(sf::RenderTarget& target)
{
    // update geometry
    if(m_geometryUpdate)
    {
        switch(m_type)
        {
            case Type::Horizontal: m_size = sf::Vector2f(m_length, 20.f); break;
            case Type::Vertical: m_size = sf::Vector2f(20.f, m_length); break;
            default: CRASH(); break;
        }

        m_geometryUpdate = false;
    }

    Widget::render(target);
}

sf::FloatRect ScrollBar::getKnobBounds()
{
    sf::FloatRect rect(0.f, 0.f, 20.f, 20.f);
    double knobPos = (m_value / m_maxValue) * (m_length - 60.0) + 20.0;

    switch(m_type)
    {
        case Type::Horizontal: rect.left = knobPos; break;
        case Type::Vertical: rect.top = knobPos; break;
        default: CRASH(); break;
    }

    return rect;
}

void ScrollBar::renderOnly(sf::RenderTarget& target)
{
    // background
    sf::RectangleShape rs(m_size);
    target.draw(rs);

    // knob
    sf::FloatRect rect = getKnobBounds();
    rs.setSize(rect.getSize());
    rs.setPosition(rect.getPosition());

    if(m_knobDragged)
        rs.setFillColor(sf::Color::Red);
    else
        rs.setFillColor(sf::Color::Green);

    target.draw(rs);
}

}
