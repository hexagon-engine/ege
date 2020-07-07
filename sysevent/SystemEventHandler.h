/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#pragma once

#include "SystemEvent.h"

#include <loop/EventHandler.h>

namespace EGE
{

class SystemEventHandler : public EventHandler
{
public:
    virtual EventResult handle(Event& event) override;

    virtual void onClose() {}
    virtual void onResize(sf::Event::SizeEvent& event) {}
    virtual void onLossFocus() {}
    virtual void onGainFocus() {}
    virtual void onTextEnter(sf::Event::TextEvent& event) {}
    virtual void onKeyPress(sf::Event::KeyEvent& event) {}
    virtual void onKeyRelease(sf::Event::KeyEvent& event) {}
    virtual void onMouseWheelScroll(sf::Event::MouseWheelScrollEvent& event) {}
    virtual void onMouseButtonPress(sf::Event::MouseButtonEvent& event) {}
    virtual void onMouseButtonRelease(sf::Event::MouseButtonEvent& event) {}
    virtual void onMouseMove(sf::Event::MouseMoveEvent& event) {}
    virtual void onMouseEnter() {}
    virtual void onMouseLeave() {}
    virtual void onJoystickButtonPress(sf::Event::JoystickButtonEvent& event) {}
    virtual void onJoystickButtonRelease(sf::Event::JoystickButtonEvent& event) {}
    virtual void onJoystickMove(sf::Event::JoystickMoveEvent& event) {}
    virtual void onJoystickConnect(sf::Event::JoystickConnectEvent& event) {}
    virtual void onJoystickDisconnect(sf::Event::JoystickConnectEvent& event) {}
    virtual void onTouchBegin(sf::Event::TouchEvent& event) {}
    virtual void onTouchMove(sf::Event::TouchEvent& event) {}
    virtual void onTouchEnd(sf::Event::TouchEvent& event) {}
    virtual void onSensorChange(sf::Event::SensorEvent& event) {}
};

}
