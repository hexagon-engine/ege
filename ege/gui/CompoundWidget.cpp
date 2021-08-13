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

/*
#include "GUIScreen.h"

#include "GUIGameLoop.h"

#include <ege/debug/Logger.h>

namespace EGE
{

void CompoundWidget::onResize(sf::Event::SizeEvent& event)
{
    ege_log.debug() << "CompoundWidget::onResize(" << event.width << "," << event.height << ") on " << getId();
    for(auto widget: m_childWidgets)
    {
        ege_log.debug() << "  Widget " << widget->getId();
        ASSERT(widget);
        widget->onResize(event);
    }
}

void CompoundWidget::onTextEnter(sf::Event::TextEvent& event)
{
    for(auto widget: m_childWidgets)
    {
        ASSERT(widget);
        if(widget->isHidden())
            continue;
        widget->onTextEnter(event);
    }
}

void CompoundWidget::onKeyPress(sf::Event::KeyEvent& event)
{
    for(auto widget: m_childWidgets)
    {
        ASSERT(widget);
        if(widget->isHidden())
            continue;
        widget->onKeyPress(event);
    }
}

void CompoundWidget::onKeyRelease(sf::Event::KeyEvent& event)
{
    for(auto widget: m_childWidgets)
    {
        ASSERT(widget);
        if(widget->isHidden())
            continue;
        widget->onKeyRelease(event);
    }
}

void CompoundWidget::onMouseLeave()
{
    // generate some event outside the window
    sf::Event::MouseMoveEvent event;
    event.x = -10;
    event.y = -10;
    onMouseMove(event);
}

void CompoundWidget::onJoystickButtonPress(sf::Event::JoystickButtonEvent& event)
{
    for(auto widget: m_childWidgets)
    {
        if(widget->isHidden())
            continue;
        widget->onJoystickButtonPress(event);
    }
}

void CompoundWidget::onJoystickButtonRelease(sf::Event::JoystickButtonEvent& event)
{
    for(auto widget: m_childWidgets)
    {
        if(widget->isHidden())
            continue;
        widget->onJoystickButtonRelease(event);
    }
}

void CompoundWidget::onJoystickMove(sf::Event::JoystickMoveEvent& event)
{
    for(auto widget: m_childWidgets)
    {
        if(widget->isHidden())
            continue;
        widget->onJoystickMove(event);
    }
}

void CompoundWidget::onJoystickConnect(sf::Event::JoystickConnectEvent& event)
{
    for(auto widget: m_childWidgets)
    {
        if(widget->isHidden())
            continue;
        widget->onJoystickConnect(event);
    }
}

void CompoundWidget::onJoystickDisconnect(sf::Event::JoystickConnectEvent& event)
{
    for(auto widget: m_childWidgets)
    {
        if(widget->isHidden())
            continue;
        widget->onJoystickDisconnect(event);
    }
}

void CompoundWidget::onTouchBegin(sf::Event::TouchEvent& event)
{
    for(auto widget: m_childWidgets)
    {
        if(widget->isHidden())
            continue;
        widget->onTouchBegin(event);
    }
}

void CompoundWidget::onTouchMove(sf::Event::TouchEvent& event)
{
    for(auto widget: m_childWidgets)
    {
        if(widget->isHidden())
            continue;
        widget->onTouchMove(event);
    }
}

void CompoundWidget::onTouchEnd(sf::Event::TouchEvent& event)
{
    for(auto widget: m_childWidgets)
    {
        if(widget->isHidden())
            continue;
        widget->onTouchEnd(event);
    }
}

void CompoundWidget::onSensorChange(sf::Event::SensorEvent& event)
{
    for(auto widget: m_childWidgets)
    {
        if(widget->isHidden())
            continue;
        widget->onSensorChange(event);
    }
}

void CompoundWidget::onUpdate(long long tickCounter)
{
    Widget::onUpdate(tickCounter);
    for(auto widget: m_childWidgets)
    {
        widget->onUpdate(tickCounter);
    }
}

}
*/
