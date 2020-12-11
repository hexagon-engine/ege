/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#include "Button.h"

#include <ege/gui/GUIGameLoop.h>

namespace EGE
{

void Button::onMouseButtonRelease(sf::Event::MouseButtonEvent& event)
{
    handleClick(EGE::Vec2d(event.x, event.y));
    Widget::onMouseButtonRelease(event); //it removes m_leftClicked state
}

void Button::onTouchEnd(sf::Event::TouchEvent& event)
{
    handleClick(EGE::Vec2d(event.x, event.y));
    Widget::onTouchEnd(event); //it removes m_leftClicked state
}

void Button::renderOnly(Renderer& renderer)
{
    sf::RenderTarget& target = renderer.getTarget();

    // base
    if(m_leftClicked)
    {
        sf::RectangleShape rs;
        rs.setOutlineThickness(1.f);

        rs.setFillColor(sf::Color(192, 192, 192));
        rs.setSize(sf::Vector2f(m_size.x, m_size.y) - sf::Vector2f(2.f, 2.f));
        rs.setPosition(2.f, 2.f);
        rs.setOutlineColor(sf::Color(29, 29, 29));
        target.draw(rs);

        rs.setFillColor(sf::Color::Transparent);
        rs.setSize(sf::Vector2f(m_size.x, m_size.y) - sf::Vector2f(1.f, 1.f));
        rs.setPosition(1.f, 1.f);
        rs.setOutlineColor(sf::Color(200, 200, 200));
        target.draw(rs);

        rs.setSize(sf::Vector2f(m_size.x, m_size.y) - sf::Vector2f(1.f, 1.f));
        rs.setPosition(0.f, 0.f);
        rs.setOutlineColor(sf::Color(255, 255, 255));
        target.draw(rs);
    }
    else if(m_mouseOver)
    {
        sf::RectangleShape rs;
        rs.setOutlineThickness(1.f);

        rs.setFillColor(sf::Color(210, 210, 210));
        rs.setSize(sf::Vector2f(m_size.x, m_size.y) - sf::Vector2f(2.f, 2.f));
        rs.setPosition(1.f, 1.f);
        rs.setOutlineColor(sf::Color(255, 255, 255));
        target.draw(rs);

        rs.setFillColor(sf::Color::Transparent);
        rs.setSize(sf::Vector2f(m_size.x, m_size.y) - sf::Vector2f(1.f, 1.f));
        rs.setPosition(0.f, 0.f);
        rs.setOutlineColor(sf::Color(29, 29, 29));
        target.draw(rs);
    }
    else
    {
        sf::RectangleShape rs;
        rs.setOutlineThickness(1.f);

        rs.setFillColor(sf::Color(200, 200, 200));
        rs.setSize(sf::Vector2f(m_size.x, m_size.y) - sf::Vector2f(2.f, 2.f));
        rs.setPosition(1.f, 1.f);
        rs.setOutlineColor(sf::Color(255, 255, 255));
        target.draw(rs);

        rs.setFillColor(sf::Color::Transparent);
        rs.setSize(sf::Vector2f(m_size.x, m_size.y) - sf::Vector2f(1.f, 1.f));
        rs.setPosition(0.f, 0.f);
        rs.setOutlineColor(sf::Color(29, 29, 29));
        target.draw(rs);
    }

    // label
    auto font = m_parent->getLoop().getResourceManager()->getDefaultFont();
    ASSERT(font);
    sf::Text text(m_label, *font, 18);
    text.setPosition((int)m_size.x / 2, (int)m_size.y / 2);
    text.setOrigin((int)text.getLocalBounds().width / 2, (int)text.getLocalBounds().height / 2);
    text.setFillColor(sf::Color::Black);
    target.draw(text);
}

void Button::handleClick(EGE::Vec2d position)
{
    if(m_leftClicked)
    {
        onClick(position);
        if(m_callback)
            m_callback();
    }
}

void Button::onClick(EGE::Vec2d)
{
    m_parent->onCommand(Command(this));
}

}
