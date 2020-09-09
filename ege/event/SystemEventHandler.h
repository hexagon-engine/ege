/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#pragma once

#include "SystemEvent.h"

#include <ege/loop/EventHandler.h>

namespace EGE
{

class SystemEventHandler : public EventHandler
{
public:
    virtual EventResult handle(Event& event) override;

    virtual void onClose() {}
    virtual void onResize(sf::Event::SizeEvent&) {}
    virtual void onLossFocus() {}
    virtual void onGainFocus() {}
    virtual void onTextEnter(sf::Event::TextEvent&) {}
    virtual void onKeyPress(sf::Event::KeyEvent&) {}
    virtual void onKeyRelease(sf::Event::KeyEvent&) {}
    virtual void onMouseWheelScroll(sf::Event::MouseWheelScrollEvent&) {}
    virtual void onMouseButtonPress(sf::Event::MouseButtonEvent&) {}
    virtual void onMouseButtonRelease(sf::Event::MouseButtonEvent&) {}
    virtual void onMouseMove(sf::Event::MouseMoveEvent&) {}
    virtual void onMouseEnter() {}
    virtual void onMouseLeave() {}
    virtual void onJoystickButtonPress(sf::Event::JoystickButtonEvent&) {}
    virtual void onJoystickButtonRelease(sf::Event::JoystickButtonEvent&) {}
    virtual void onJoystickMove(sf::Event::JoystickMoveEvent&) {}
    virtual void onJoystickConnect(sf::Event::JoystickConnectEvent&) {}
    virtual void onJoystickDisconnect(sf::Event::JoystickConnectEvent&) {}
    virtual void onTouchBegin(sf::Event::TouchEvent&) {}
    virtual void onTouchMove(sf::Event::TouchEvent&) {}
    virtual void onTouchEnd(sf::Event::TouchEvent&) {}
    virtual void onSensorChange(sf::Event::SensorEvent&) {}
};

}
