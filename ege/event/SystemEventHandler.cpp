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

#include "SystemEventHandler.h"

#include <ege/debug/Logger.h>
#include <iostream>

namespace EGE
{

EventResult SystemEventHandler::handle(Event& event)
{
    SystemEvent* sysEvent = EventCast<SystemEvent>(&event);
    ASSERT(sysEvent);
    sf::Event sfEvent = sysEvent->getEvent();
    //std::cerr << "SystemEventHandler: " << sfEvent.type << std::endl;
    switch(sfEvent.type)
    {
    case sf::Event::Closed:
        {
            onClose();
        } break;
    case sf::Event::Resized:
        {
            onResize(sfEvent.size);
        } break;
    case sf::Event::LostFocus:
        {
            onLossFocus();
        } break;
    case sf::Event::GainedFocus:
        {
            onGainFocus();
        } break;
    case sf::Event::TextEntered:
        {
            onTextEnter(sfEvent.text);
        } break;
    case sf::Event::KeyPressed:
        {
            onKeyPress(sfEvent.key);
        } break;
    case sf::Event::KeyReleased:
        {
            onKeyRelease(sfEvent.key);
        } break;
    case sf::Event::MouseWheelMoved: //SFML backward compability
        break;
    case sf::Event::MouseWheelScrolled:
        {
            onMouseWheelScroll(sfEvent.mouseWheelScroll);
        } break;
    case sf::Event::MouseButtonPressed:
        {
            onMouseButtonPress(sfEvent.mouseButton);
        } break;
    case sf::Event::MouseButtonReleased:
        {
            onMouseButtonRelease(sfEvent.mouseButton);
        } break;
    case sf::Event::MouseMoved:
        {
            onMouseMove(sfEvent.mouseMove);
        } break;
    case sf::Event::MouseEntered:
        {
            onMouseEnter();
        } break;
    case sf::Event::MouseLeft:
        {
            onMouseLeave();
        } break;
    case sf::Event::JoystickButtonPressed:
        {
            onJoystickButtonPress(sfEvent.joystickButton);
        } break;
    case sf::Event::JoystickButtonReleased:
        {
            onJoystickButtonRelease(sfEvent.joystickButton);
        } break;
    case sf::Event::JoystickMoved:
        {
            onJoystickMove(sfEvent.joystickMove);
        } break;
    case sf::Event::JoystickConnected:
        {
            onJoystickConnect(sfEvent.joystickConnect);
        } break;
    case sf::Event::JoystickDisconnected:
        {
            onJoystickDisconnect(sfEvent.joystickConnect);
        } break;
    case sf::Event::TouchBegan:
        {
            onTouchBegin(sfEvent.touch);
        } break;
    case sf::Event::TouchMoved:
        {
            onTouchMove(sfEvent.touch);
        } break;
    case sf::Event::TouchEnded:
        {
            onTouchEnd(sfEvent.touch);
        } break;
    case sf::Event::SensorChanged:
        {
            onSensorChange(sfEvent.sensor);
        } break;
    default:
        CRASH();
        return EventResult::Failure;
    }
    return EventResult::Success;
}

}
