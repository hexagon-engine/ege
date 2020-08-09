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
    handleClick(sf::Vector2f(event.x, event.y));
    Widget::onMouseButtonRelease(event); //it removes m_leftClicked state
}

void Button::onTouchEnd(sf::Event::TouchEvent& event)
{
    handleClick(sf::Vector2f(event.x, event.y));
    Widget::onTouchEnd(event); //it removes m_leftClicked state
}

void Button::render(sf::RenderTarget& target)
{
    Widget::render(target);
    auto font = m_parent->getLoop()->getResourceManager().lock()->getDefaultFont();
    ASSERT(font);
    sf::Text text(m_label, *font, 18);
    text.setFillColor(sf::Color::Black);
    text.setPosition(m_size / 2.f);
    text.setOrigin(text.getLocalBounds().width / 2.f, text.getLocalBounds().height / 2.f);
    target.draw(text);
}

void Button::handleClick(sf::Vector2f position)
{
    if(m_leftClicked)
        m_parent->onCommand(Command(this));
}

}
