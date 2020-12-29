/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#pragma once

#include <ege/gfx/RenderStates.h>

#include "CompoundWidget.h"

#define EGE_MODAL_DIALOG_REPLACED -8

namespace EGE
{

class GUIScreen : public CompoundWidget
{
public:
    explicit GUIScreen(GUIGameLoop& gameLoop)
    : CompoundWidget(gameLoop) {}

    // for modal dialogs
    explicit GUIScreen(Widget& parent)
    : CompoundWidget(parent) {}

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

    virtual void onUpdate(long long tickCounter) override;
    virtual void onUnload() {}

    // Args: dialog, exitCode
    virtual void onDialogExit(GUIScreen*, int) {}

    virtual void doRender(Renderer& renderer, const RenderStates& states = {}) override;

    // If you call it on dialog, the onDialogExit function is called
    // and the dialog is removed on parent.
    virtual void exitDialog(int code);

    // Opened modal dialog should have 'this' as parent.
    // e.g   openDialog(make<MyDialog>(this));
    virtual void openDialog(SharedPtr<GUIScreen> dialog);

    virtual SharedPtr<GUIScreen> getDialog() { return m_dialog; }

protected:
    SharedPtr<GUIScreen> m_dialog;
};

}
