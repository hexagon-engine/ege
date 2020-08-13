/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#include "GUIScreen.h"

#include "GUIGameLoop.h"

namespace EGE
{

void GUIScreen::onClose()
{
    DefaultSystemEventHandler::onClose();
    // TODO: change meaning of these events to be widget-relative?
    /*for(auto widget: m_childWidgets)
    {
        widget->onClose();
    }*/
}

void GUIScreen::onResize(sf::Event::SizeEvent& event)
{
    DefaultSystemEventHandler::onResize(event);
    for(auto widget: m_childWidgets)
    {
        ASSERT(widget);
        widget->onResize(event);
    }
}

void GUIScreen::onLossFocus()
{
    DefaultSystemEventHandler::onLossFocus();
    // TODO: change meaning of these events to be widget-relative?
    /*for(auto widget: m_childWidgets)
    {
        widget->onLossFocus();
    }*/
}

void GUIScreen::onGainFocus()
{
    DefaultSystemEventHandler::onGainFocus();
    // TODO: change meaning of these events to be widget-relative?
    /*for(auto widget: m_childWidgets)
    {
        widget->onGainFocus();
    }*/
}

void GUIScreen::onTextEnter(sf::Event::TextEvent& event)
{
    DefaultSystemEventHandler::onTextEnter(event);
    for(auto widget: m_childWidgets)
    {
        ASSERT(widget);
        widget->onTextEnter(event);
    }
}

void GUIScreen::onKeyPress(sf::Event::KeyEvent& event)
{
    DefaultSystemEventHandler::onKeyPress(event);
    for(auto widget: m_childWidgets)
    {
        ASSERT(widget);
        widget->onKeyPress(event);
    }
}

void GUIScreen::onKeyRelease(sf::Event::KeyEvent& event)
{
    DefaultSystemEventHandler::onKeyRelease(event);
    for(auto widget: m_childWidgets)
    {
        ASSERT(widget);
        widget->onKeyRelease(event);
    }
}

void GUIScreen::onMouseWheelScroll(sf::Event::MouseWheelScrollEvent& event)
{
    DefaultSystemEventHandler::onMouseWheelScroll(event);
    for(auto widget: m_childWidgets)
    {
        ASSERT(widget);
        widget->onMouseWheelScroll(event);
    }
}

void GUIScreen::onMouseButtonPress(sf::Event::MouseButtonEvent& event)
{
    DefaultSystemEventHandler::onMouseButtonPress(event);
    for(auto widget: m_childWidgets)
    {
        ASSERT(widget);
        widget->onMouseButtonPress(event);
    }
}

void GUIScreen::onMouseButtonRelease(sf::Event::MouseButtonEvent& event)
{
    DefaultSystemEventHandler::onMouseButtonRelease(event);
    for(auto widget: m_childWidgets)
    {
        ASSERT(widget);
        widget->onMouseButtonRelease(event);
    }
}

void GUIScreen::onMouseMove(sf::Event::MouseMoveEvent& event)
{
    DefaultSystemEventHandler::onMouseMove(event);
    for(auto widget: m_childWidgets)
    {
        ASSERT(widget);
        widget->onMouseMove(event);
    }
}

void GUIScreen::onMouseEnter()
{
    DefaultSystemEventHandler::onMouseEnter();
    // TODO: change meaning of these events to be widget-relative?
    /*for(auto widget: m_childWidgets)
    {
        widget->onMouseEnter();
    }*/
}

void GUIScreen::onMouseLeave()
{
    DefaultSystemEventHandler::onMouseLeave();
    // TODO: change meaning of these events to be widget-relative?
    /*for(auto widget: m_childWidgets)
    {
        widget->onMouseLeave();
    }*/
}

void GUIScreen::onJoystickButtonPress(sf::Event::JoystickButtonEvent& event)
{
    DefaultSystemEventHandler::onJoystickButtonPress(event);
    for(auto widget: m_childWidgets)
    {
        widget->onJoystickButtonPress(event);
    }
}

void GUIScreen::onJoystickButtonRelease(sf::Event::JoystickButtonEvent& event)
{
    DefaultSystemEventHandler::onJoystickButtonRelease(event);
    for(auto widget: m_childWidgets)
    {
        widget->onJoystickButtonRelease(event);
    }
}

void GUIScreen::onJoystickMove(sf::Event::JoystickMoveEvent& event)
{
    DefaultSystemEventHandler::onJoystickMove(event);
    for(auto widget: m_childWidgets)
    {
        widget->onJoystickMove(event);
    }
}

void GUIScreen::onJoystickConnect(sf::Event::JoystickConnectEvent& event)
{
    DefaultSystemEventHandler::onJoystickConnect(event);
    for(auto widget: m_childWidgets)
    {
        widget->onJoystickConnect(event);
    }
}

void GUIScreen::onJoystickDisconnect(sf::Event::JoystickConnectEvent& event)
{
    DefaultSystemEventHandler::onJoystickDisconnect(event);
    for(auto widget: m_childWidgets)
    {
        widget->onJoystickDisconnect(event);
    }
}

void GUIScreen::onTouchBegin(sf::Event::TouchEvent& event)
{
    DefaultSystemEventHandler::onTouchBegin(event);
    for(auto widget: m_childWidgets)
    {
        widget->onTouchBegin(event);
    }
}

void GUIScreen::onTouchMove(sf::Event::TouchEvent& event)
{
    DefaultSystemEventHandler::onTouchMove(event);
    for(auto widget: m_childWidgets)
    {
        widget->onTouchMove(event);
    }
}

void GUIScreen::onTouchEnd(sf::Event::TouchEvent& event)
{
    DefaultSystemEventHandler::onTouchEnd(event);
    for(auto widget: m_childWidgets)
    {
        widget->onTouchEnd(event);
    }
}

void GUIScreen::onSensorChange(sf::Event::SensorEvent& event)
{
    DefaultSystemEventHandler::onSensorChange(event);
    for(auto widget: m_childWidgets)
    {
        widget->onSensorChange(event);
    }
}

void GUIScreen::onUpdate(long long tickCounter)
{
    for(auto widget: m_childWidgets)
    {
        widget->onUpdate(tickCounter);
    }
}

void GUIScreen::render(sf::RenderTarget& target)
{
    // TODO: draw only visible widgets
    setPosition(sf::Vector2f());
    m_size = sf::Vector2f(target.getSize());
    for(auto widget: m_childWidgets)
    {
        widget->render(target);
    }
}

void GUIScreen::addWidget(std::shared_ptr<Widget> widget)
{
    DUMP(GUI_DEBUG, "addWidget");
    // deferredInvoke to allow adding and removing widgets inside event handlers
    getLoop()->deferredInvoke([this,widget]() {
        DUMP(GUI_DEBUG, widget.get());
        ASSERT(widget.get());
        widget->onLoad();

        // allow widgets know about window's size when creating
        sf::Vector2u wndSize = getLoop()->getWindow().lock()->getSize();
        sf::Event::SizeEvent event{wndSize.x, wndSize.y};
        widget->onResize(event);

        m_childWidgets.push_back(widget);
    });
}

void GUIScreen::removeWidget(Widget* widget)
{
    // deferredInvoke to allow adding and removing widgets inside event handlers
    getLoop()->deferredInvoke([this,widget]() {
        for(auto it = m_childWidgets.begin(); it != m_childWidgets.end(); it++)
        {
            if(it->get() == widget)
            {
                m_childWidgets.erase(it);
                return;
            }
        }
    });
}

}
