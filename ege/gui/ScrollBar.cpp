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
        double newVal = (rv - 20.0) / (m_length - 55.0);

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
    double knobPos = (m_value / m_maxValue) * (m_length - 55.0) + 20.0;

    switch(m_type)
    {
        case Type::Horizontal: rect.left = knobPos; rect.width = 15.f; break;
        case Type::Vertical: rect.top = knobPos; rect.height = 15.f; break;
        default: CRASH(); break;
    }

    return rect;
}

void drawButtonLike(sf::RenderTarget& target, sf::Vector2f position, sf::Vector2f size)
{
    sf::RectangleShape rs;
    rs.setOutlineThickness(1.f);

    rs.setFillColor(sf::Color(209, 209, 209));
    rs.setSize(size - sf::Vector2f(2.f, 2.f));
    rs.setPosition(position + sf::Vector2f(1.f, 1.f));
    rs.setOutlineColor(sf::Color(255, 255, 255));
    target.draw(rs);

    rs.setFillColor(sf::Color::Transparent);
    rs.setSize(size - sf::Vector2f(1.f, 1.f));
    rs.setPosition(position);
    rs.setOutlineColor(sf::Color(60, 60, 60));
    target.draw(rs);
}

void drawRect(sf::RenderTarget& target, sf::Vector2f position, sf::Vector2f size, sf::Color color)
{
    sf::RectangleShape rs(size);
    rs.setFillColor(color);
    rs.setPosition(position);
    target.draw(rs);
}

void ScrollBar::renderOnly(sf::RenderTarget& target)
{
    // background
    drawRect(target, sf::Vector2f(0.f, 20.f), m_size - sf::Vector2f(0.f, 40.f), sf::Color(175, 175, 175));

    // scroll buttons
    drawButtonLike(target, sf::Vector2f(0.f, 0.f), sf::Vector2f(20.f, 20.f));
    drawButtonLike(target, sf::Vector2f(0.f, m_size.y - 20.f), sf::Vector2f(20.f, 20.f));

    // knob
    sf::FloatRect rect = getKnobBounds();
    drawButtonLike(target, rect.getPosition(), rect.getSize());
}

}
