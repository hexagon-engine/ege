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

#pragma once

#include "SystemEvent.h"

#include <ege/loop/EventHandler.h>

namespace EGE
{

class SystemEventHandler : public SimpleEventHandler<SystemEvent>
{
public:
    SystemEventHandler()
    : SimpleEventHandler({}) {}

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
