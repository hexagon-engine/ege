/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
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
    EGE::Vec2d tmp1(event.x, event.y);
    log(LogLevel::Debug) << "ScrollBar::onMouseButtonPress: " << tmp1.x << ", " << tmp1.y;
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

void ScrollBar::render(sf::RenderTarget& target, const RenderStates&)
{
    // update geometry
    if(m_geometryUpdate)
    {
        switch(m_type)
        {
            case Type::Horizontal: m_size = EGE::Vec2d(m_length, 20.f); break;
            case Type::Vertical: m_size = EGE::Vec2d(20.f, m_length); break;
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

void ScrollBar::renderOnly(sf::RenderTarget& target, const RenderStates&)
{
    Renderer renderer(target);

    // background
    renderer.renderRectangle(0.0, 20.0, m_size.x, m_size.y - 40.0, sf::Color(175, 175, 175));

    // scroll buttons
    renderer.renderButtonLike(0.0, 0.0, 20.0, 20.0);
    renderer.renderButtonLike(0.0, m_size.y - 20.0, 20.0, 20.0);

    // knob
    sf::FloatRect rect = getKnobBounds();
    renderer.renderButtonLike(rect.left, rect.top, rect.width, rect.height);
}

}
