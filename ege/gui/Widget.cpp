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
, m_parent(nullptr) {}

sf::FloatRect Widget::getBoundingBox()
{
    return sf::FloatRect(m_position, m_size);
}

void Widget::render(sf::RenderTarget& target)
{
    // set view
    sf::View view(sf::FloatRect(m_position, m_size));
    sf::Vector2u windowSize = target.getSize();
    view.setViewport(sf::FloatRect(m_position.x / windowSize.x, m_position.y / windowSize.y,
                                   m_size.x / windowSize.x, m_size.y / windowSize.y));
    target.setView(view);
    DUMP(0, view.getSize().x);
    DUMP(0, view.getSize().y);

    // draw some debug shape
    if constexpr(WIDGET_DEBUG)
    {
        sf::RectangleShape rs(m_size);
        rs.setPosition(m_position);
        if(m_mouseOver)
            rs.setFillColor(sf::Color::Red);
        if(m_leftClicked)
            rs.setFillColor(sf::Color::Green);
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

}
