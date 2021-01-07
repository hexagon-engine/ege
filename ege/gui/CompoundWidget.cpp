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

#include "GUIScreen.h"

#include "GUIGameLoop.h"

#include <ege/debug/Logger.h>

namespace EGE
{

void CompoundWidget::onResize(sf::Event::SizeEvent& event)
{
    log(LogLevel::Debug) << "CompoundWidget::onResize(" << event.width << "," << event.height << ") on " << getId();
    for(auto widget: m_childWidgets)
    {
        log(LogLevel::Debug) << "  Widget " << widget->getId();
        ASSERT(widget);
        widget->onResize(event);
    }
}

void CompoundWidget::onTextEnter(sf::Event::TextEvent& event)
{
    for(auto widget: m_childWidgets)
    {
        ASSERT(widget);
        widget->onTextEnter(event);
    }
}

void CompoundWidget::onKeyPress(sf::Event::KeyEvent& event)
{
    for(auto widget: m_childWidgets)
    {
        ASSERT(widget);
        widget->onKeyPress(event);
    }
}

void CompoundWidget::onKeyRelease(sf::Event::KeyEvent& event)
{
    for(auto widget: m_childWidgets)
    {
        ASSERT(widget);
        widget->onKeyRelease(event);
    }
}

void CompoundWidget::onMouseWheelScroll(sf::Event::MouseWheelScrollEvent& event)
{
    for(auto widget: m_childWidgets)
    {
        ASSERT(widget);
        widget->onMouseWheelScroll(event);
    }
}

void CompoundWidget::onMouseButtonPress(sf::Event::MouseButtonEvent& event)
{
    Vec2d position = Vec2d(event.x, event.y);
    log(LogLevel::Debug) << "CompoundWidget::onMouseButtonPress(" << position.x << "," << position.y << ")";
    for(auto widget: m_childWidgets)
    {
        ASSERT(widget);

        // Change focused widget.
        log(LogLevel::Debug) << "  Widget " << widget->getId() << " pos(" << widget->getAbsolutePosition().x << "," << widget->getAbsolutePosition().y << ")"
        << " size(" << widget->getSize().x << "," << widget->getSize().y << ")";
        if(widget->isMouseOver(position) && event.button == sf::Mouse::Left)
        {
            log(LogLevel::Debug) << "- isMouseOver!";
            if(m_focusedWidget)
                m_focusedWidget->onLossFocus();

            m_focusedWidget = widget.get();
            m_focusedWidget->onGainFocus();

            sf::Event::MouseButtonEvent event2 { event.button, (int)position.x, (int)position.y };
            widget->onMouseButtonPress(event2);
        }
    }
}

void CompoundWidget::onMouseButtonRelease(sf::Event::MouseButtonEvent& event)
{
    EGE::Vec2d position = Vec2d(event.x, event.y);
    for(auto widget: m_childWidgets)
    {
        ASSERT(widget);
        sf::Event::MouseButtonEvent event2 { event.button, (int)position.x, (int)position.y };
        widget->onMouseButtonRelease(event2);
    }
}

void CompoundWidget::onMouseMove(sf::Event::MouseMoveEvent& event)
{
    Vec2d position = Vec2d(event.x, event.y);
    for(auto widget: m_childWidgets)
    {
        ASSERT(widget);
        sf::Event::MouseMoveEvent event2 { (int)position.x, (int)position.y };
        widget->onMouseMove(event2);
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
        widget->onJoystickButtonPress(event);
    }
}

void CompoundWidget::onJoystickButtonRelease(sf::Event::JoystickButtonEvent& event)
{
    for(auto widget: m_childWidgets)
    {
        widget->onJoystickButtonRelease(event);
    }
}

void CompoundWidget::onJoystickMove(sf::Event::JoystickMoveEvent& event)
{
    for(auto widget: m_childWidgets)
    {
        widget->onJoystickMove(event);
    }
}

void CompoundWidget::onJoystickConnect(sf::Event::JoystickConnectEvent& event)
{
    for(auto widget: m_childWidgets)
    {
        widget->onJoystickConnect(event);
    }
}

void CompoundWidget::onJoystickDisconnect(sf::Event::JoystickConnectEvent& event)
{
    for(auto widget: m_childWidgets)
    {
        widget->onJoystickDisconnect(event);
    }
}

void CompoundWidget::onTouchBegin(sf::Event::TouchEvent& event)
{
    for(auto widget: m_childWidgets)
    {
        widget->onTouchBegin(event);
    }
}

void CompoundWidget::onTouchMove(sf::Event::TouchEvent& event)
{
    for(auto widget: m_childWidgets)
    {
        widget->onTouchMove(event);
    }
}

void CompoundWidget::onTouchEnd(sf::Event::TouchEvent& event)
{
    for(auto widget: m_childWidgets)
    {
        widget->onTouchEnd(event);
    }
}

void CompoundWidget::onSensorChange(sf::Event::SensorEvent& event)
{
    for(auto widget: m_childWidgets)
    {
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

void CompoundWidget::doRender(Renderer& renderer, const RenderStates& states)
{
    log(LogLevel::Debug) << "CompoundWidget::doRender(" << renderer.getTarget().getSize().x << "," << renderer.getTarget().getSize().y << ")";
    // TODO: render background??
    // TODO: draw only visible widgets

    // Render child widgets
    for(auto widget: m_childWidgets)
    {
        setCustomView(renderer.getTarget());
        log(LogLevel::Debug) << "-- View: (" << renderer.getTarget().getView().getSize().x << "," << renderer.getTarget().getView().getSize().y << ")";
        widget->doRender(renderer, states);
    }

    // Render widget self
    Widget::doRender(renderer, states);
}

void CompoundWidget::addWidget(SharedPtr<Widget> widget)
{
    // deferredInvoke to allow adding and removing widgets inside event handlers
    deferredInvoke([this, widget]() {
        DUMP(GUI_DEBUG, "addWidget");
        DUMP(GUI_DEBUG, widget.get());
        ASSERT(widget);
        widget->onLoad();

        // allow widgets know about window's size when creating
        sf::Vector2u wndSize = getLoop().getWindow().getSize();
        sf::Event::SizeEvent event{wndSize.x, wndSize.y};
        widget->onResize(event);

        m_childWidgets.push_back(widget);
        setGeometryNeedUpdate();
    });
}

void CompoundWidget::removeWidget(Widget* widget)
{
    // deferredInvoke to allow adding and removing widgets inside event handlers
    deferredInvoke([this, widget]() {
        for(auto it = m_childWidgets.begin(); it != m_childWidgets.end(); it++)
        {
            if(it->get() == widget)
            {
                if(widget == m_focusedWidget)
                    m_focusedWidget = nullptr;

                m_childWidgets.erase(it);
                return;
            }
        }
        setGeometryNeedUpdate();
    });
}

void CompoundWidget::updateLayout()
{
    Widget::updateLayout();
    for(auto widget: m_childWidgets)
    {
        if(widget->geometryNeedUpdate())
            widget->updateLayout();
    }
}

}
