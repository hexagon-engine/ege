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

#include "KeybindManager.h"

#include <ege/core/Component.h>
#include <ege/debug/Logger.h>

namespace EGE
{

bool Input::operator==(const Input& other) const
{
    if(type != other.type)
        return false;

    switch(type)
    {
        case Keyboard: return value.key == other.value.key;
        case JoystickButton: return value.joystick.id == other.value.joystick.id && value.joystick.button == other.value.joystick.button;
        case JoystickAxis: return value.joystick.id == other.value.joystick.id && value.joystick.axis == other.value.joystick.axis;
        case MouseButton: return value.mouseButton == other.value.mouseButton;
        case MouseWheel: return value.mouseWheel == other.value.mouseWheel;
        case KeyPair: return value.keyPair.minus == other.value.keyPair.minus || value.keyPair.plus == other.value.keyPair.plus;
        default: CRASH_WITH_MESSAGE("Unknown input type!");
    }
}

void KeybindHandler::addTriggerHandler(StringView name, TriggerKeybindHandler handler)
{
    m_triggerKBs.insert(std::make_pair(name, handler));
}

void KeybindHandler::addSwitchHandler(StringView name, SwitchKeybindHandler handler)
{
    m_switchKBs.insert(std::make_pair(name, handler));
}

void KeybindHandler::addStrengthHandler(StringView name, StrengthKeybindHandler handler)
{
    m_strengthKBs.insert(std::make_pair(name, handler));
}

void KeybindHandler::onKeyPress(sf::Event::KeyEvent& event)
{
    ege_log.debug() << "KeybindManager::onKeyPress " << (int)event.code;
    callAllTriggerKBs(Input(event.code));
    callAllSwitchKBs(Input(event.code), true);
    callAllKeyPairs(false, event.code);
}

void KeybindHandler::onKeyRelease(sf::Event::KeyEvent& event)
{
    ege_log.debug() << "KeybindManager::onKeyRelease " << (int)event.code;
    callAllSwitchKBs(Input(event.code), false);
    callAllKeyPairs(true, event.code);
}

void KeybindHandler::onMouseWheelScroll(sf::Event::MouseWheelScrollEvent& event)
{
    ege_log.debug() << "KeybindManager::onMouseWheelScroll " << (int)event.wheel << ";" << event.delta;
    if(event.delta > 0)
    {
        callAllTriggerKBs(Input(event.wheel));
        callAllSwitchKBs(Input(event.wheel), true);
    }
    else
        callAllSwitchKBs(Input(event.wheel), false);

    callAllStrengthKBs(Input(event.wheel), event.delta);
}

void KeybindHandler::onMouseButtonPress(sf::Event::MouseButtonEvent& event)
{
    ege_log.debug() << "KeybindManager::onMouseButtonPress " << (int)event.button;
    callAllTriggerKBs(Input(event.button));
    callAllSwitchKBs(Input(event.button), true);
}

void KeybindHandler::onMouseButtonRelease(sf::Event::MouseButtonEvent& event)
{
    ege_log.debug() << "KeybindManager::onMouseButtonRelease " << (int)event.button;
    callAllSwitchKBs(Input(event.button), false);
}

void KeybindHandler::onJoystickButtonPress(sf::Event::JoystickButtonEvent& event)
{
    ege_log.debug() << "KeybindManager::onJoystickButtonPress " << event.joystickId << ";" << event.button;
    callAllTriggerKBs(Input(event.joystickId, event.button));
    callAllSwitchKBs(Input(event.joystickId, event.button), true);
}

void KeybindHandler::onJoystickButtonRelease(sf::Event::JoystickButtonEvent& event)
{
    ege_log.debug() << "KeybindManager::onJoystickButtonRelease " << event.joystickId << ";" << event.button;
    callAllSwitchKBs(Input(event.joystickId, event.button), false);
}

void KeybindHandler::onJoystickMove(sf::Event::JoystickMoveEvent& event)
{
    ege_log.debug() << "KeybindManager::JoystickMoveEvent " << event.joystickId << ";" << (int)event.axis << ";" << event.position;
    callAllStrengthKBs(Input(event.joystickId, event.axis), event.position / 100.0);
    if(event.position > 0)
    {
        callAllTriggerKBs(Input(event.joystickId, event.axis));
        callAllSwitchKBs(Input(event.joystickId, event.axis), true);
    }
    else
        callAllSwitchKBs(Input(event.joystickId, event.axis), false);
}

void KeybindHandler::callAllTriggerKBs(Input const& input)
{
    for(auto& kb: m_triggerKBs)
    {
        if(m_manager->ensureKeybind(kb.first) == input)
            kb.second();
    }
}

void KeybindHandler::callAllSwitchKBs(Input const& input, Boolean value)
{
    for(auto& kb: m_switchKBs)
    {
        if(m_manager->ensureKeybind(kb.first) == input)
            kb.second(value);
    }
}

void KeybindHandler::callAllStrengthKBs(Input const& input, Float value)
{
    for(auto& kb: m_strengthKBs)
    {
        if(m_manager->ensureKeybind(kb.first) == input)
            kb.second(value);
    }
}

void KeybindHandler::callAllKeyPairs(bool release, sf::Keyboard::Key key)
{
    for(auto& it: m_strengthKBs)
    {
        auto input = m_manager->ensureKeybind(it.first);
        if(input.type == Input::KeyPair)
        {
            auto minus = input.value.keyPair.minus;
            auto plus = input.value.keyPair.plus;
            bool isMinusPressed = sf::Keyboard::isKeyPressed(minus) || (key == minus && !release);
            bool isPlusPressed = sf::Keyboard::isKeyPressed(plus) || (key == plus && !release);

            // TODO: Make it configurable somehow
            if(isMinusPressed == isPlusPressed)
                it.second(0);
            else if(isMinusPressed)
                it.second(-1);
            else if(isPlusPressed)
                it.second(1);
        }
    }
}

bool KeybindManager::load(StringView)
{
    NOT_IMPLEMENTED("KeybindManager loading");
}

bool KeybindManager::save(StringView)
{
    NOT_IMPLEMENTED("KeybindManager saving");
}

void KeybindManager::setKeybind(StringView name, Input const& input)
{
    m_keybinds.insert(std::make_pair(name, input));
}

Input KeybindManager::ensureKeybind(StringView name) const
{
    auto it = m_keybinds.find(String(name));
    ASSERT(it != m_keybinds.end());
    return it->second;
}

Optional<Input> KeybindManager::getKeybind(StringView name) const
{
    auto it = m_keybinds.find(String(name));
    if(it != m_keybinds.end())
        return it->second;
    return {};
}

KeybindHandler& KeybindManager::hook(SharedPtr<KeybindManager> const& manager, EventTarget& target)
{
    auto keybindHandler = wrapUnique(new KeybindHandler(manager));
    auto keybindHandlerRaw = keybindHandler.get();
    target.events<SystemEvent>().addHandler(std::move(keybindHandler));
    return *keybindHandlerRaw;
}

}

