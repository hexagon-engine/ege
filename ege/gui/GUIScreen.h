/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#pragma once

#include "Widget.h"

namespace EGE
{

class GUIScreen : public Widget
{
public:
    GUIScreen(GUIGameLoop* gameLoop)
    : Widget(gameLoop) {}

    // System Events -- are passed to all child widgets
    virtual void onClose() override;
    virtual void onResize(sf::Event::SizeEvent& event) override;
    virtual void onLossFocus() override;
    virtual void onGainFocus() override;
    virtual void onTextEnter(sf::Event::TextEvent& event) override;
    virtual void onKeyPress(sf::Event::KeyEvent& event) override;
    virtual void onKeyRelease(sf::Event::KeyEvent& event) override;
    virtual void onMouseWheelScroll(sf::Event::MouseWheelScrollEvent& event) override;
    virtual void onMouseButtonPress(sf::Event::MouseButtonEvent& event) override;
    virtual void onMouseButtonRelease(sf::Event::MouseButtonEvent& event) override;
    virtual void onMouseMove(sf::Event::MouseMoveEvent& event) override;
    virtual void onMouseEnter() override;
    virtual void onMouseLeave() override;
    virtual void onJoystickButtonPress(sf::Event::JoystickButtonEvent& event) override;
    virtual void onJoystickButtonRelease(sf::Event::JoystickButtonEvent& event) override;
    virtual void onJoystickMove(sf::Event::JoystickMoveEvent& event) override;
    virtual void onJoystickConnect(sf::Event::JoystickConnectEvent& event) override;
    virtual void onJoystickDisconnect(sf::Event::JoystickConnectEvent& event) override;
    virtual void onTouchBegin(sf::Event::TouchEvent& event) override;
    virtual void onTouchMove(sf::Event::TouchEvent& event) override;
    virtual void onTouchEnd(sf::Event::TouchEvent& event) override;
    virtual void onSensorChange(sf::Event::SensorEvent& event) override;

    // GUI-specific Events
    virtual void onCommand(int id) {}

    // Widget Events
    virtual void onUpdate(long long tickCounter) override;

    virtual void render(sf::RenderTarget& target) override;

    void addWidget(std::shared_ptr<Widget> widget);
    void removeWidget(Widget* widget);

private:
    std::vector<std::shared_ptr<Widget>> m_childWidgets;
};

}
