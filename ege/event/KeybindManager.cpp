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

#include <ege/core/EventLoop.h>
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

bool KeybindManager::load(String)
{
    NOT_IMPLEMENTED("KeybindManager loading");
}

bool KeybindManager::save(String)
{
    NOT_IMPLEMENTED("KeybindManager saving");
}

void KeybindManager::addTrigger(String name, Input defaultInput, TriggerKeybindHandler handler)
{
    m_triggerKBs.insert(std::make_pair(name, TriggerKB{defaultInput, handler}));
}

void KeybindManager::addSwitch(String name, Input defaultInput, SwitchKeybindHandler handler)
{
    m_switchKBs.insert(std::make_pair(name, SwitchKB{defaultInput, handler}));
}

void KeybindManager::addStrength(String name, Input defaultInput, StrengthKeybindHandler handler)
{
    m_strengthKBs.insert(std::make_pair(name, StrengthKB{defaultInput, handler}));
}

void KeybindManager::setTrigger(String name, Input input)
{
    auto it = m_triggerKBs.find(name);
    if(it != m_triggerKBs.end())
        it->second.input = input;
    else
        ege_log.warning() << "No keybind (trigger) in keybind manager: " << name;
}

void KeybindManager::setSwitch(String name, Input input)
{
    auto it = m_switchKBs.find(name);
    if(it != m_switchKBs.end())
        it->second.input = input;
    else
        ege_log.warning() << "No keybind (switch) in keybind manager: " << name;
}

void KeybindManager::setStrength(String name, Input input)
{
    auto it = m_strengthKBs.find(name);
    if(it != m_strengthKBs.end())
        it->second.input = input;
    else
        ege_log.warning() << "No keybind (strength) in keybind manager: " << name;
}

void KeybindManager::hook(SharedPtr<KeybindManager> manager, EventLoop& loop)
{
    loop.events<SystemEvent>().addHandler(manager);
}

void KeybindManager::onKeyPress(sf::Event::KeyEvent& event)
{
    ege_log.debug() << "KeybindManager::onKeyPress " << (int)event.code;
    callAllTriggerKBs(Input(event.code));
    callAllSwitchKBs(Input(event.code), true);
    callAllKeyPairs(false, event.code);
}

void KeybindManager::onKeyRelease(sf::Event::KeyEvent& event)
{
    ege_log.debug() << "KeybindManager::onKeyRelease " << (int)event.code;
    callAllSwitchKBs(Input(event.code), false);
    callAllKeyPairs(true, event.code);
}

void KeybindManager::onMouseWheelScroll(sf::Event::MouseWheelScrollEvent& event)
{
    ege_log.debug() << "KeybindManager::onMouseWheelScroll " << (int)event.wheel << ";" << event.delta;
    if(event.delta > 0)
    {
        callAllTriggerKBs(Input(event.wheel));
        callAllSwitchKBs(Input(event.wheel), true);
    }

    callAllStrengthKBs(Input(event.wheel), event.delta);
}

void KeybindManager::onMouseButtonPress(sf::Event::MouseButtonEvent& event)
{
    ege_log.debug() << "KeybindManager::onMouseButtonPress " << (int)event.button;
    callAllTriggerKBs(Input(event.button));
    callAllSwitchKBs(Input(event.button), true);
}

void KeybindManager::onMouseButtonRelease(sf::Event::MouseButtonEvent& event)
{
    ege_log.debug() << "KeybindManager::onMouseButtonRelease " << (int)event.button;
    callAllSwitchKBs(Input(event.button), false);
}

void KeybindManager::onJoystickButtonPress(sf::Event::JoystickButtonEvent& event)
{
    ege_log.debug() << "KeybindManager::onJoystickButtonPress " << event.joystickId << ";" << event.button;
    callAllTriggerKBs(Input(event.joystickId, event.button));
    callAllSwitchKBs(Input(event.joystickId, event.button), true);
}

void KeybindManager::onJoystickButtonRelease(sf::Event::JoystickButtonEvent& event)
{
    ege_log.debug() << "KeybindManager::onJoystickButtonRelease " << event.joystickId << ";" << event.button;
    callAllSwitchKBs(Input(event.joystickId, event.button), false);
}

void KeybindManager::onJoystickMove(sf::Event::JoystickMoveEvent& event)
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

Vector<const KeybindManager::TriggerKB*> KeybindManager::findAllTriggerKBs(Input input) const
{
    Vector<const KeybindManager::TriggerKB*> tmp;
    for(auto& it: m_triggerKBs)
        if(it.second.input == input)
            tmp.push_back(&it.second);
    return tmp;
}

Vector<const KeybindManager::SwitchKB*> KeybindManager::findAllSwitchKBs(Input input) const
{
    Vector<const KeybindManager::SwitchKB*> tmp;
    for(auto& it: m_switchKBs)
        if(it.second.input == input)
            tmp.push_back(&it.second);
    return tmp;
}

Vector<const KeybindManager::StrengthKB*> KeybindManager::findAllStrengthKBs(Input input) const
{
    Vector<const KeybindManager::StrengthKB*> tmp;
    for(auto& it: m_strengthKBs)
        if(it.second.input == input)
            tmp.push_back(&it.second);
    return tmp;
}

void KeybindManager::callAllTriggerKBs(Input input)
{
    auto vec = findAllTriggerKBs(input);
    for(auto& kb: vec)
        kb->handler();
}

void KeybindManager::callAllSwitchKBs(Input input, Boolean value)
{
    auto vec = findAllSwitchKBs(input);
    for(auto& kb: vec)
        kb->handler(value);
}

void KeybindManager::callAllStrengthKBs(Input input, Float value)
{
    auto vec = findAllStrengthKBs(input);
    for(auto& kb: vec)
        kb->handler(value);
}

void KeybindManager::callAllKeyPairs(bool release, sf::Keyboard::Key key)
{
    for(auto& it: m_strengthKBs)
    {
        if(it.second.input.type == Input::KeyPair)
        {
            auto minus = it.second.input.value.keyPair.minus;
            auto plus = it.second.input.value.keyPair.plus;
            bool isMinusPressed = sf::Keyboard::isKeyPressed(minus) || (key == minus && !release);
            bool isPlusPressed = sf::Keyboard::isKeyPressed(plus) || (key == plus && !release);

            // TODO: Make it configurable somehow
            if(isMinusPressed == isPlusPressed)
                it.second.handler(0);
            else if(isMinusPressed)
                it.second.handler(-1);
            else if(isPlusPressed)
                it.second.handler(1);
        }
    }
}

}

