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
#include "SystemEventHandler.h"

#include <ege/core/EventHandler.h>
#include <ege/core/EventTarget.h>
#include <ege/util/Optional.h>
#include <ege/util/Serializable.h>
#include <SFML/Window.hpp>

namespace EGE
{

struct Input : public Serializable
{
    enum Type
    {
        Keyboard,
        JoystickButton,
        JoystickAxis,
        MouseButton,
        MouseWheel,
        KeyPair,
        Invalid
        // TODO: mouse move
    } type;

    union Value
    {
        sf::Keyboard::Key key; // For type=Keyboard

        struct JoystickData
        {
            int id = 0;
            union
            {
                sf::Joystick::Axis axis; // For type=JoystickAxis
                int button; // For type=JoystickButton
            };
        } joystick;

        struct KeyPair
        {
            sf::Keyboard::Key minus;
            sf::Keyboard::Key plus;
        } keyPair;

        sf::Mouse::Button mouseButton; // For type=MouseButton
        sf::Mouse::Wheel mouseWheel; // For type=MouseWheel
    } value;

    Input()
    : type(Invalid), value({}) {}

    Input(sf::Keyboard::Key key)
    : type(Keyboard), value(Value{.key = key}) {}

    Input(int id, sf::Joystick::Axis axis)
    : type(JoystickAxis), value(Value{.joystick = Value::JoystickData{.id = id, .axis = axis}}) {}

    Input(int id, int button)
    : type(JoystickButton), value(Value{.joystick = Value::JoystickData{.id = id, .button = button}}) {}

    Input(sf::Mouse::Button button)
    : type(MouseButton), value(Value{.mouseButton = button}) {}

    Input(sf::Mouse::Wheel wheel)
    : type(MouseWheel), value(Value{.mouseWheel = wheel}) {}

    Input(sf::Keyboard::Key minusKey, sf::Keyboard::Key plusKey)
    : type(KeyPair), value(Value{.keyPair = Value::KeyPair{.minus = minusKey, .plus = plusKey}}) {}

    bool operator==(const Input& other) const;
    bool operator!=(const Input& other) const { return !(*this == other); }

    virtual SharedPtr<ObjectMap> serialize() const override;
    virtual bool deserialize(SharedPtr<ObjectMap>) override;
};

class ComponentBase;
class KeybindManager;

class KeybindHandler : public SystemEventHandler
{
public:
    // void handle(), event called on press.
    // For JoystickAxis/MouseWheel, called on any 'up' move (value > 0).
    // NOTE: this allows to bind scroll up to space :^)
    typedef Function<void()> TriggerKeybindHandler;

    // void handle(Boolean pressed), event called on press(pressed=true), and release(pressed=false)
    // For JoystickAxis/MouseWheel, pressed=0 for value < 0.5, pressed=1 otherwise.
    typedef Function<void(Boolean)> SwitchKeybindHandler;

    // void handle(Float strength), event called on change, strength is useful for joystick keybinds.
    // strength is always in range -1 - 1
    // For non-JoystickAxis/MouseWheel: press -> strength=1; release -> strength=0
    // For key pairs: none pressed / both pressed -> strength=0, minus pressed -> strength=-1, plus pressed -> strength=1
    typedef Function<void(Float)> StrengthKeybindHandler; // for joystick

    void addTriggerHandler(StringView name, TriggerKeybindHandler handler);
    void addSwitchHandler(StringView name, SwitchKeybindHandler handler);
    void addStrengthHandler(StringView name, StrengthKeybindHandler handler);

private:
    friend class KeybindManager;

    KeybindHandler(SharedPtr<KeybindManager> const& manager)
    : m_manager(manager) { ASSERT(manager); }

    virtual void onKeyPress(sf::Event::KeyEvent&) override;
    virtual void onKeyRelease(sf::Event::KeyEvent&) override;
    virtual void onMouseWheelScroll(sf::Event::MouseWheelScrollEvent&) override;
    virtual void onMouseButtonPress(sf::Event::MouseButtonEvent&) override;
    virtual void onMouseButtonRelease(sf::Event::MouseButtonEvent&) override;
    virtual void onJoystickButtonPress(sf::Event::JoystickButtonEvent&) override;
    virtual void onJoystickButtonRelease(sf::Event::JoystickButtonEvent&) override;
    virtual void onJoystickMove(sf::Event::JoystickMoveEvent&) override;

    void callAllTriggerKBs(Input const&);
    void callAllSwitchKBs(Input const&, Boolean value);
    void callAllStrengthKBs(Input const&, Float value);
    void callAllKeyPairs(bool release, sf::Keyboard::Key key);

    StringMap<TriggerKeybindHandler> m_triggerKBs;
    StringMap<SwitchKeybindHandler> m_switchKBs;
    StringMap<StrengthKeybindHandler> m_strengthKBs;

    SharedPtr<KeybindManager> m_manager;
};

class KeybindManager
{
public:
    // TODO: add API for mouse moves

    // Uses last file name if name not given. The default is 'keybinds.json'
    // The fileName is relative to 'config' dir.
    bool load(StringView fileName = ""sv);

    // Last file name, or 'keybinds.json' is used by default.
    bool save(StringView fileName = ""sv);

    // It's ignored if already exists.
    void addKeybind(StringView name, Input const&);

    // It overrides old value if exists.
    void setKeybind(StringView name, Input const&);

    Input ensureKeybind(StringView name) const;
    Optional<Input> getKeybind(StringView name) const;

    static KeybindHandler& hook(SharedPtr<KeybindManager> const&, EventTarget&);

    // TODO: Allow iteration for 'controls' GUI
private:
    String m_lastFileName { "keybinds.json" };
    UnorderedStringMap<Input> m_keybinds;
};

}
