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
    if(m_dialog)
    {
        m_dialog->onClose();
        // we must notify all (global event)
    }
    DefaultSystemEventHandler::onClose();
    CompoundWidget::onClose();
}

void GUIScreen::onResize(sf::Event::SizeEvent& event)
{
    if(m_dialog)
    {
        m_dialog->onResize(event);
        // we must notify all (global event)
    }
    DefaultSystemEventHandler::onResize(event);
    CompoundWidget::onResize(event);
}

void GUIScreen::onLossFocus()
{
    if(m_dialog)
    {
        m_dialog->onLossFocus();
        // we must notify all (global event)
    }
    DefaultSystemEventHandler::onLossFocus();
    CompoundWidget::onLossFocus();
}

void GUIScreen::onGainFocus()
{
    if(m_dialog)
    {
        m_dialog->onGainFocus();
        // we must notify all (global event)
    }
    DefaultSystemEventHandler::onGainFocus();
    CompoundWidget::onGainFocus();
}

void GUIScreen::onTextEnter(sf::Event::TextEvent& event)
{
    if(m_dialog)
    {
        m_dialog->onTextEnter(event);
        return;
    }
    DefaultSystemEventHandler::onTextEnter(event);
    CompoundWidget::onTextEnter(event);
}

void GUIScreen::onKeyPress(sf::Event::KeyEvent& event)
{
    if(m_dialog)
    {
        m_dialog->onKeyPress(event);
        return;
    }
    DefaultSystemEventHandler::onKeyPress(event);
    CompoundWidget::onKeyPress(event);
}

void GUIScreen::onKeyRelease(sf::Event::KeyEvent& event)
{
    if(m_dialog)
    {
        m_dialog->onKeyRelease(event);
        return;
    }
    DefaultSystemEventHandler::onKeyRelease(event);
    CompoundWidget::onKeyRelease(event);
}

void GUIScreen::onMouseWheelScroll(sf::Event::MouseWheelScrollEvent& event)
{
    if(m_dialog)
    {
        m_dialog->onMouseWheelScroll(event);
        return;
    }
    DefaultSystemEventHandler::onMouseWheelScroll(event);
    CompoundWidget::onMouseWheelScroll(event);
}

void GUIScreen::onMouseButtonPress(sf::Event::MouseButtonEvent& event)
{
    if(m_dialog)
    {
        m_dialog->onMouseButtonPress(event);
        return;
    }
    DefaultSystemEventHandler::onMouseButtonPress(event);
    CompoundWidget::onMouseButtonPress(event);
}

void GUIScreen::onMouseButtonRelease(sf::Event::MouseButtonEvent& event)
{
    if(m_dialog)
    {
        m_dialog->onMouseButtonRelease(event);
        return;
    }
    DefaultSystemEventHandler::onMouseButtonRelease(event);
    CompoundWidget::onMouseButtonRelease(event);
}

void GUIScreen::onMouseMove(sf::Event::MouseMoveEvent& event)
{
    if(m_dialog)
    {
        m_dialog->onMouseMove(event);
        return;
    }
    DefaultSystemEventHandler::onMouseMove(event);
    CompoundWidget::onMouseMove(event);
}

void GUIScreen::onMouseEnter()
{
    if(m_dialog)
    {
        m_dialog->onMouseEnter();
        return;
    }
    DefaultSystemEventHandler::onMouseEnter();
    CompoundWidget::onMouseEnter();
}

void GUIScreen::onMouseLeave()
{
    if(m_dialog)
    {
        m_dialog->onMouseLeave();
        return;
    }
    DefaultSystemEventHandler::onMouseLeave();
    CompoundWidget::onMouseLeave();
}

void GUIScreen::onJoystickButtonPress(sf::Event::JoystickButtonEvent& event)
{
    if(m_dialog)
    {
        m_dialog->onJoystickButtonPress(event);
        return;
    }
    DefaultSystemEventHandler::onJoystickButtonPress(event);
    CompoundWidget::onJoystickButtonPress(event);
}

void GUIScreen::onJoystickButtonRelease(sf::Event::JoystickButtonEvent& event)
{
    if(m_dialog)
    {
        m_dialog->onJoystickButtonRelease(event);
        return;
    }
    DefaultSystemEventHandler::onJoystickButtonRelease(event);
    CompoundWidget::onJoystickButtonRelease(event);
}

void GUIScreen::onJoystickMove(sf::Event::JoystickMoveEvent& event)
{
    if(m_dialog)
    {
        m_dialog->onJoystickMove(event);
        return;
    }
    DefaultSystemEventHandler::onJoystickMove(event);
    CompoundWidget::onJoystickMove(event);
}

void GUIScreen::onJoystickConnect(sf::Event::JoystickConnectEvent& event)
{
    if(m_dialog)
    {
        m_dialog->onJoystickConnect(event);
        return;
    }
    DefaultSystemEventHandler::onJoystickConnect(event);
    CompoundWidget::onJoystickConnect(event);
}

void GUIScreen::onJoystickDisconnect(sf::Event::JoystickConnectEvent& event)
{
    if(m_dialog)
    {
        m_dialog->onJoystickDisconnect(event);
        return;
    }
    DefaultSystemEventHandler::onJoystickDisconnect(event);
    CompoundWidget::onJoystickDisconnect(event);
}

void GUIScreen::onTouchBegin(sf::Event::TouchEvent& event)
{
    if(m_dialog)
    {
        m_dialog->onTouchBegin(event);
        return;
    }
    DefaultSystemEventHandler::onTouchBegin(event);
    CompoundWidget::onTouchBegin(event);
}

void GUIScreen::onTouchMove(sf::Event::TouchEvent& event)
{
    if(m_dialog)
    {
        m_dialog->onTouchMove(event);
        return;
    }
    DefaultSystemEventHandler::onTouchMove(event);
    CompoundWidget::onTouchMove(event);
}

void GUIScreen::onTouchEnd(sf::Event::TouchEvent& event)
{
    if(m_dialog)
    {
        m_dialog->onTouchEnd(event);
        return;
    }
    DefaultSystemEventHandler::onTouchEnd(event);
    CompoundWidget::onTouchEnd(event);
}

void GUIScreen::onSensorChange(sf::Event::SensorEvent& event)
{
    if(m_dialog)
    {
        m_dialog->onSensorChange(event);
        return;
    }
    DefaultSystemEventHandler::onSensorChange(event);
    CompoundWidget::onSensorChange(event);
}

void GUIScreen::onUpdate(long long tickCounter)
{
    CompoundWidget::onUpdate(tickCounter);
    if(m_dialog)
    {
        m_dialog->onUpdate(tickCounter);
    }
}

void GUIScreen::render(sf::RenderTarget& target, const RenderStates& states)
{
    // Ensure that GUI is resized to cover entire target
    setPosition(EGE::Vec2d());
    auto size = target.getSize();
    m_size = EGE::Vec2d(size.x, size.y);

    // Actually draw child widgets
    CompoundWidget::render(target, states);

    // Render dialog on top of GUI.
    if(m_dialog)
    {
        m_dialog->render(target, states);
    }

}

void GUIScreen::exitDialog(int code)
{
    deferredInvoke([this, code] {
        GUIScreen* parent = (GUIScreen*)m_parent;
        ASSERT(parent);
        parent->onDialogExit(this, code);
        onUnload();
        parent->m_dialog = nullptr;
    });
}

void GUIScreen::openDialog(SharedPtr<GUIScreen> dialog)
{
    // We use here a property of deferredInvokes
    // - they have guaranteed the order of calling
    if(m_dialog)
        m_dialog->exitDialog(EGE_MODAL_DIALOG_REPLACED);

    deferredInvoke([this, dialog] {
        m_dialog = dialog;
        m_dialog->onLoad();

        sf::Vector2u wndSize = getLoop()->getWindow().lock()->getSize();
        sf::Event::SizeEvent event{wndSize.x, wndSize.y};
        m_dialog->onResize(event);
    });
}

}
