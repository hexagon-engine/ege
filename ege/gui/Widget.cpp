/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#include "Widget.h"

#include "GUIGameLoop.h"

#include <ege/main/Config.h>
#include <SFML/Graphics.hpp>

namespace EGE
{

Widget::Widget(GUIGameLoop* gameLoop)
: DefaultSystemEventHandler(gameLoop->getWindow())
, m_parent(nullptr)
, m_gameLoop(gameLoop) {}

sf::FloatRect Widget::getBoundingBox()
{
    return sf::FloatRect(m_position, m_size);
}

void Widget::render(sf::RenderTarget& target)
{
    setViewForWidget(target);

    // draw some debug shape
    if constexpr(WIDGET_DEBUG)
    {
        sf::RectangleShape rs(m_size - sf::Vector2f(2.f, 2.f));
        rs.setPosition(sf::Vector2f(1.f, 1.f));
        rs.setOutlineColor(sf::Color::Cyan);
        rs.setOutlineThickness(1.f);
        rs.setFillColor(sf::Color::Transparent);
        if(m_mouseOver)
            rs.setOutlineColor(sf::Color::Red);
        if(m_leftClicked)
            rs.setOutlineColor(sf::Color::Green);
        target.draw(rs);
    }
}

void Widget::onMouseMove(sf::Event::MouseMoveEvent& event)
{
    m_mouseOver = isMouseOver(sf::Vector2f(event.x, event.y));
    if(!m_mouseOver)
        m_leftClicked = false;
}


void Widget::onMouseButtonPress(sf::Event::MouseButtonEvent& event)
{
    if(isMouseOver(sf::Vector2f(event.x, event.y)) && event.button == sf::Mouse::Left)
    {
        m_leftClicked = true;
    }
}

void Widget::onMouseButtonRelease(sf::Event::MouseButtonEvent& event)
{
    if(event.button == sf::Mouse::Left)
    {
        m_leftClicked = false;
    }
}

bool Widget::isMouseOver(sf::Vector2f position)
{
    return getBoundingBox().contains(position);
}

void Widget::setViewForWidget(sf::RenderTarget& target)
{
    sf::Vector2f parentPosition;

    DUMP(WIDGET_DEBUG, m_parent);
    if(m_parent)
        parentPosition = (sf::Vector2f)m_parent->getPosition();

    sf::View view(sf::FloatRect(sf::Vector2f(), m_size));

    DUMP(WIDGET_DEBUG, m_size.x);
    DUMP(WIDGET_DEBUG, m_size.y);

    sf::Vector2u windowSize;
    windowSize = target.getSize();

    sf::Vector2f widgetPosition = parentPosition + getPosition();

    DUMP(WIDGET_DEBUG, parentPosition.x);
    DUMP(WIDGET_DEBUG, parentPosition.y);
    DUMP(WIDGET_DEBUG, widgetPosition.x);
    DUMP(WIDGET_DEBUG, widgetPosition.y);

    view.setViewport(sf::FloatRect(widgetPosition.x / windowSize.x, widgetPosition.y / windowSize.y,
                                   m_size.x / windowSize.x, m_size.y / windowSize.y));

    DUMP(WIDGET_DEBUG, widgetPosition.x / windowSize.x);
    DUMP(WIDGET_DEBUG, widgetPosition.y / windowSize.y);
    DUMP(WIDGET_DEBUG, m_size.x / windowSize.x);
    DUMP(WIDGET_DEBUG, m_size.y / windowSize.y);
    target.setView(view);
}

}
