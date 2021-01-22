/*
*
*   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*
*      ,----  ,----  ,----
*      |      |      |
*      |----  | --,  |----
*      |      |   |  |
*      '----  '---'  '----
*
*     Framework Library for Hexagon
*
*    Copyright (c) Sppmacd 2020 - 2021
*
*    Permission is hereby granted, free of charge, to any person obtaining a copy
*    of this software and associated documentation files (the "Software"), to deal
*    in the Software without restriction, including without limitation the rights
*    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
*    copies of the Software, and to permit persons to whom the Software is
*    furnished to do so, subject to the following conditions:
*
*    The above copyright notice and this permission notice shall be included in all
*    copies or substantial portions of the Software.
*
*    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
*    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
*    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
*    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
*    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
*    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
*    SOFTWARE.
*
*   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*
*/

#include "Widget.h"

#include "GUIGameLoop.h"

#include <ege/debug/Logger.h>
#include <ege/main/Config.h>
#include <ege/util/PointerUtils.h>
#include <SFML/Graphics.hpp>

namespace EGE
{

Widget::Widget(GUIGameLoop& gameLoop, String id)
: Animatable(&gameLoop, id)
, DefaultSystemEventHandler(gameLoop.getWindow())
, LayoutElement(nullptr, id)
, m_gameLoop(gameLoop)
, m_parentWidget(nullptr) {}

sf::FloatRect Widget::getBoundingBox()
{
    return sf::FloatRect(getAbsolutePosition().x, getAbsolutePosition().y, getSize().x, getSize().y);
}
sf::FloatRect Widget::getViewport(sf::RenderTarget& target) const
{
    DUMP(0, getSize().x);
    DUMP(0, getSize().y);

    sf::Vector2u windowSize;
    windowSize = target.getSize();

    EGE::Vec2d widgetPosition = getAbsolutePosition();

    DUMP(0, widgetPosition.x);
    DUMP(0, widgetPosition.y);

    sf::FloatRect currentRect(widgetPosition.x / windowSize.x, widgetPosition.y / windowSize.y,
                  getSize().x / windowSize.x, getSize().y / windowSize.y);

    if(getParentWidget())
    {
        sf::FloatRect intersection;
        currentRect.intersects((sf::FloatRect)getParentWidget()->getViewport(target), intersection);
        return intersection;
    }
    else
        return currentRect;
}

void Widget::render(Renderer& renderer) const
{
    // draw some debug shape
    if constexpr(WIDGET_DEBUG)
    {
        // TODO: Use Renderer's methods
        sf::RectangleShape rs(sf::Vector2f(getSize().x, getSize().y) - sf::Vector2f(2.f, 2.f));
        rs.setPosition(sf::Vector2f(1.f, 1.f));
        rs.setOutlineColor(sf::Color::Cyan);

        if(hasFocus())
        {
            rs.setOutlineThickness(2.f);
            rs.setSize(sf::Vector2f(getSize().x, getSize().y) - sf::Vector2f(4.f, 4.f));
            rs.setPosition(sf::Vector2f(2.f, 2.f));
        }
        else
            rs.setOutlineThickness(1.f);

        rs.setFillColor(sf::Color::Transparent);
        if(m_mouseOver)
            rs.setOutlineColor(sf::Color::Red);
        if(m_leftClicked)
            rs.setOutlineColor(sf::Color::Green);
        renderer.getTarget().draw(rs);

        //renderer.renderText(0, 0, *getLoop().getResourceManager()->getDefaultFont(), std::to_string(getPosition().x) + "," + std::to_string(getPosition().y), 10, sf::Color::Black);
    }
}

void Widget::onResize(sf::Event::SizeEvent&)
{
    setGeometryNeedUpdate();
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

bool Widget::isMouseOver(EGE::Vec2d position)
{
    return getBoundingBox().contains(sf::Vector2f(position.x, position.y));
}

sf::View Widget::getCustomView(sf::RenderTarget& target) const
{
    sf::FloatRect viewport = getViewport(target);
    sf::View view(sf::FloatRect(sf::Vector2f(),
                                sf::Vector2f(viewport.getSize().x * target.getSize().x, viewport.getSize().y * target.getSize().y)));
    view.setViewport(viewport);
    return view;
}

void Widget::onUpdate(long long tickCounter)
{
    (void)tickCounter;
    EventLoop::onUpdate();
}

void Widget::updateLayout()
{
}

}
