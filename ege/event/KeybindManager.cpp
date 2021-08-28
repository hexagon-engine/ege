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
#include <ege/util/CommonPaths.h>
#include <ege/util/JSONConverter.h>
#include <ege/util/system/FileSystem.h>

#include <SFML/Window.hpp>
#include <fstream>

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

SharedPtr<ObjectMap> Input::serialize() const
{
    auto object = make<ObjectMap>();

    auto typeToString = [](Type type) {
        switch(type)
        {
            case Keyboard:
                return "Keyboard"sv;
            case JoystickButton:
                return "JoystickButton"sv;
            case JoystickAxis:
                return "JoystickAxis"sv;
            case MouseButton:
                return "MouseButton"sv;
            case MouseWheel:
                return "MouseWheel"sv;
            case KeyPair:
                return "KeyPair"sv;
            default:
                CRASH();
        }
    };

    object->addString("type", String(typeToString(type)));
    switch(type)
    {
        case Keyboard:
            object->addInt("key", value.key);
            break;
        case JoystickButton:
            object->addInt("id", value.joystick.id);
            object->addInt("button", value.joystick.button);
            break;
        case JoystickAxis:
            object->addInt("id", value.joystick.id);
            object->addUnsignedInt("axis", value.joystick.axis);
            break;
        case MouseButton:
            object->addUnsignedInt("button", value.mouseButton);
            break;
        case MouseWheel:
            object->addUnsignedInt("wheel", value.mouseWheel);
            break;
        case KeyPair:
            object->addInt("minus", value.keyPair.minus);
            object->addInt("plus", value.keyPair.plus);
            break;
        default:
            CRASH();
    }
    return object;
}

bool Input::deserialize(SharedPtr<ObjectMap> object)
{
    auto typeFromString = [](String const& type) {
        if(type == "Keyboard"sv)
            return Keyboard;
        if(type == "JoystickButton"sv)
            return JoystickButton;
        if(type == "JoystickAxis"sv)
            return JoystickAxis;
        if(type == "MouseButton"sv)
            return MouseButton;
        if(type == "MouseWheel"sv)
            return MouseWheel;
        if(type == "KeyPair"sv)
            return KeyPair;
        return Invalid;
    };

    type = typeFromString(object->get("type").asString().valueOr(""));
    if(type == Invalid)
        return false;

    switch(type)
    {
        case Keyboard:
            value.key = static_cast<sf::Keyboard::Key>(object->get("key").asInt().valueOr(-1));
            break;
        case JoystickButton:
            value.joystick.id = object->get("id").asInt().valueOr(0);
            value.joystick.button = object->get("button").asInt().valueOr(0);
            break;
        case JoystickAxis:
            value.joystick.id = object->get("id").asInt().valueOr(0);
            value.joystick.axis = static_cast<sf::Joystick::Axis>(object->get("axis").asUnsignedInt().valueOr(0));
            break;
        case MouseButton:
            value.mouseButton = static_cast<sf::Mouse::Button>(object->get("button").asUnsignedInt().valueOr(0));
            break;
        case MouseWheel:
            value.mouseWheel = static_cast<sf::Mouse::Wheel>(object->get("wheel").asUnsignedInt().valueOr(0));
            break;
        case KeyPair:
            value.keyPair.minus = static_cast<sf::Keyboard::Key>(object->get("minus").asInt().valueOr(-1));
            value.keyPair.plus = static_cast<sf::Keyboard::Key>(object->get("plus").asInt().valueOr(-1));
            break;
        default:
            return false;
    }
    return true;
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

bool KeybindManager::load(StringView fileName)
{
    if(!fileName.empty() && fileName != m_lastFileName)
        m_lastFileName = fileName;

    ege_log.info() << "Loading keybinds from " << m_lastFileName;
    std::ifstream file(CommonPaths::configDir() + "/" + String{m_lastFileName});
    if(!file.good())
        return false;

    SharedPtr<Object> object;
    if(!(file >> objectIn(object, JSONConverter())))
        return false;

    auto objectMap = ObjectValue(object).asMap();
    if(!objectMap.hasValue())
        return false;

    for(auto it: objectMap.value())
    {
        auto name = it.first;
        auto inputMap = it.second.asMap();
        if(!inputMap.hasValue())
            return false;

        Input input;
        if(!input.deserialize(inputMap.value().map()))
            return false;

        setKeybind(name, input);
    }

    return true;
}

bool KeybindManager::save(StringView fileName)
{
    if(!fileName.empty() && fileName != m_lastFileName)
        m_lastFileName = fileName;

    ege_log.info() << "Saving keybinds to " << m_lastFileName;
    System::createPath(CommonPaths::configDir());
    std::ofstream file(CommonPaths::configDir() + "/" + String{m_lastFileName});
    if(!file.good())
        return false;
    
    SharedPtr<ObjectMap> object = make<ObjectMap>();

    for(auto& keybind: m_keybinds)
        object->add(keybind.first, keybind.second);

    if(!(file << objectOut(*object, JSONConverter())))
        return false;

    return true;
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

