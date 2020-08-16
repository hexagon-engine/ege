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
sf::FloatRect Widget::getViewport(sf::RenderTarget& target)
{
    sf::Vector2f parentPosition;

    DUMP(0, m_parent);
    if(m_parent)
    {
        parentPosition = (sf::Vector2f)m_parent->getPosition();
    }

    DUMP(0, m_size.x);
    DUMP(0, m_size.y);

    sf::Vector2u windowSize;
    windowSize = target.getSize();

    sf::Vector2f widgetPosition = parentPosition + getPosition();

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
    sf::FloatRect viewport = getViewport(target);
    sf::View view(sf::FloatRect(sf::Vector2f(),
                                sf::Vector2f(viewport.getSize().x * target.getSize().x, viewport.getSize().y * target.getSize().y)));
    view.setViewport(viewport);
    target.setView(view);
}

void Widget::addAnimation(std::shared_ptr<Animation> animation, std::function<void(Animation*,double)> callback)
{
    if(!animation->getUpdateCallback())
    {
        animation->setUpdateCallback([callback](std::string name, Timer* timer) {
                                    Animation* anim = (Animation*)timer;
                                    double time = (timer->getElapsedTime().getValue()) / (timer->getInterval().getValue());
                                    if(time < 0.0)
                                        return;
                                    double val = anim->getValue(time);
                                    DUMP(1, val);
                                    callback(anim, val);
                                });
    }

    Timer* timer = new Timer(this, Timer::Mode::Limited, animation->getDelay());
    timer->setCallback([animation, this](std::string name, Timer* timer) { addTimer("Widget Animation", animation); });
    addTimer("Widget Animation Delay", timer);
}

}
