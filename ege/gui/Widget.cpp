/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#include "Widget.h"

#include "GUIGameLoop.h"

#include <ege/main/Config.h>
#include <ege/util/PointerUtils.h>
#include <SFML/Graphics.hpp>

namespace EGE
{

Widget::Widget(GUIGameLoop* gameLoop)
: DefaultSystemEventHandler(gameLoop->getWindow())
, m_parent(nullptr)
, m_gameLoop(gameLoop) {}

sf::FloatRect Widget::getBoundingBox()
{
    return sf::FloatRect(m_position.x, m_position.y, m_size.x, m_size.y);
}
sf::FloatRect Widget::getViewport(sf::RenderTarget& target)
{
    EGE::Vec2d parentPosition;

    DUMP(0, m_parent);
    if(m_parent)
    {
        parentPosition = (EGE::Vec2d)m_parent->getPosition();
    }

    DUMP(0, m_size.x);
    DUMP(0, m_size.y);

    sf::Vector2u windowSize;
    windowSize = target.getSize();

    EGE::Vec2d widgetPosition = parentPosition + getPosition();

    DUMP(0, parentPosition.x);
    DUMP(0, parentPosition.y);
    DUMP(0, widgetPosition.x);
    DUMP(0, widgetPosition.y);

    sf::FloatRect currentRect(widgetPosition.x / windowSize.x, widgetPosition.y / windowSize.y,
                  m_size.x / windowSize.x, m_size.y / windowSize.y);

    if(m_parent)
    {
        sf::FloatRect intersection;
        currentRect.intersects((sf::FloatRect)m_parent->getViewport(target), intersection);
        return intersection;
    }
    else
        return currentRect;
}

void Widget::render(sf::RenderTarget& target, const RenderStates& states)
{
    setViewForWidget(target);
    renderOnly(target, states);
}

void Widget::renderOnly(sf::RenderTarget& target, const RenderStates&)
{
    // draw some debug shape
    if constexpr(WIDGET_DEBUG)
    {
        sf::RectangleShape rs(sf::Vector2f(m_size.x, m_size.y) - sf::Vector2f(2.f, 2.f));
        rs.setPosition(sf::Vector2f(1.f, 1.f));
        rs.setOutlineColor(sf::Color::Cyan);

        if(hasFocus())
        {
            rs.setOutlineThickness(2.f);
            rs.setSize(sf::Vector2f(m_size.x, m_size.y) - sf::Vector2f(4.f, 4.f));
            rs.setPosition(sf::Vector2f(2.f, 2.f));
        }
        else
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
    bool mouseOver = isMouseOver(EGE::Vec2d(event.x, event.y));
    if(mouseOver)
    {
        if(!m_mouseOver)
            onMouseEnter();
    }
    else
    {
        m_leftClicked = false;
        if(m_mouseOver)
            onMouseLeave();
    }
    m_mouseOver = mouseOver;
}


void Widget::onMouseButtonPress(sf::Event::MouseButtonEvent& event)
{
    if(event.button == sf::Mouse::Left)
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

void Widget::onLossFocus()
{
    m_hasFocus = false;
}

void Widget::onGainFocus()
{
    m_hasFocus = true;
}

bool Widget::isMouseOver(EGE::Vec2d position)
{
    return getBoundingBox().contains(sf::Vector2f(position.x, position.y));
}

sf::View Widget::getView(sf::RenderTarget& target)
{
    sf::FloatRect viewport = getViewport(target);
    sf::View view(sf::FloatRect(sf::Vector2f(),
                                sf::Vector2f(viewport.getSize().x * target.getSize().x, viewport.getSize().y * target.getSize().y)));
    view.setViewport(viewport);
    return view;
}

void Widget::setViewForWidget(sf::RenderTarget& target)
{
    target.setView(getView(target));
}

void Widget::onUpdate(long long tickCounter)
{
    (void)tickCounter;
    EventLoop::onUpdate();
}

}
