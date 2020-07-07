#include "SystemEventHandler.h"

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
        return EventResult::Failure;
    }
    return EventResult::Success;
}

}
