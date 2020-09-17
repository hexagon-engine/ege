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
    CompoundWidget::onClose();
}

void GUIScreen::onResize(sf::Event::SizeEvent& event)
{
    DefaultSystemEventHandler::onResize(event);
    CompoundWidget::onResize(event);
}

void GUIScreen::onLossFocus()
{
    DefaultSystemEventHandler::onLossFocus();
    CompoundWidget::onLossFocus();
}

void GUIScreen::onGainFocus()
{
    DefaultSystemEventHandler::onGainFocus();
    CompoundWidget::onGainFocus();
}

void GUIScreen::onTextEnter(sf::Event::TextEvent& event)
{
    DefaultSystemEventHandler::onTextEnter(event);
    CompoundWidget::onTextEnter(event);
}

void GUIScreen::onKeyPress(sf::Event::KeyEvent& event)
{
    DefaultSystemEventHandler::onKeyPress(event);
    CompoundWidget::onKeyPress(event);
}

void GUIScreen::onKeyRelease(sf::Event::KeyEvent& event)
{
    DefaultSystemEventHandler::onKeyRelease(event);
    CompoundWidget::onKeyRelease(event);
}

void GUIScreen::onMouseWheelScroll(sf::Event::MouseWheelScrollEvent& event)
{
    DefaultSystemEventHandler::onMouseWheelScroll(event);
    CompoundWidget::onMouseWheelScroll(event);
}

void GUIScreen::onMouseButtonPress(sf::Event::MouseButtonEvent& event)
{
    DefaultSystemEventHandler::onMouseButtonPress(event);
    CompoundWidget::onMouseButtonPress(event);
}

void GUIScreen::onMouseButtonRelease(sf::Event::MouseButtonEvent& event)
{
    DefaultSystemEventHandler::onMouseButtonRelease(event);
    CompoundWidget::onMouseButtonRelease(event);
}

void GUIScreen::onMouseMove(sf::Event::MouseMoveEvent& event)
{
    DefaultSystemEventHandler::onMouseMove(event);
    CompoundWidget::onMouseMove(event);
}

void GUIScreen::onMouseEnter()
{
    DefaultSystemEventHandler::onMouseEnter();
    CompoundWidget::onMouseEnter();
}

void GUIScreen::onMouseLeave()
{
    DefaultSystemEventHandler::onMouseLeave();
    CompoundWidget::onMouseLeave();
}

void GUIScreen::onJoystickButtonPress(sf::Event::JoystickButtonEvent& event)
{
    DefaultSystemEventHandler::onJoystickButtonPress(event);
    CompoundWidget::onJoystickButtonPress(event);
}

void GUIScreen::onJoystickButtonRelease(sf::Event::JoystickButtonEvent& event)
{
    DefaultSystemEventHandler::onJoystickButtonRelease(event);
    CompoundWidget::onJoystickButtonRelease(event);
}

void GUIScreen::onJoystickMove(sf::Event::JoystickMoveEvent& event)
{
    DefaultSystemEventHandler::onJoystickMove(event);
    CompoundWidget::onJoystickMove(event);
}

void GUIScreen::onJoystickConnect(sf::Event::JoystickConnectEvent& event)
{
    DefaultSystemEventHandler::onJoystickConnect(event);
    CompoundWidget::onJoystickConnect(event);
}

void GUIScreen::onJoystickDisconnect(sf::Event::JoystickConnectEvent& event)
{
    DefaultSystemEventHandler::onJoystickDisconnect(event);
    CompoundWidget::onJoystickDisconnect(event);
}

void GUIScreen::onTouchBegin(sf::Event::TouchEvent& event)
{
    DefaultSystemEventHandler::onTouchBegin(event);
    CompoundWidget::onTouchBegin(event);
}

void GUIScreen::onTouchMove(sf::Event::TouchEvent& event)
{
    DefaultSystemEventHandler::onTouchMove(event);
    CompoundWidget::onTouchMove(event);
}

void GUIScreen::onTouchEnd(sf::Event::TouchEvent& event)
{
    DefaultSystemEventHandler::onTouchEnd(event);
    CompoundWidget::onTouchEnd(event);
}

void GUIScreen::onSensorChange(sf::Event::SensorEvent& event)
{
    DefaultSystemEventHandler::onSensorChange(event);
    CompoundWidget::onSensorChange(event);
}

void GUIScreen::render(sf::RenderTarget& target)
{
    // Ensure that GUI is resized to cover entire target
    setPosition(sf::Vector2f());
    m_size = sf::Vector2f(target.getSize());

    // Actually draw child widgets
    CompoundWidget::render(target);
}
}
