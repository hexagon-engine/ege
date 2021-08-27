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
#include <SFML/Window.hpp>

namespace EGE
{

struct Input
{
    enum Type
    {
        Keyboard,
        JoystickButton,
        JoystickAxis,
        MouseButton,
        MouseWheel,
        KeyPair
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
};

class ComponentBase;

class KeybindManager : public SystemEventHandler
{
public:
    // void handle(), event called on press.
    // For JoystickAxis/MouseWheel, called on any 'up' move (value > 0).
    // NOTE: this allows to bind scroll up to space :^)
    typedef std::function<void()> TriggerKeybindHandler;

    // void handle(Boolean pressed), event called on press(pressed=true), and release(pressed=false)
    // For JoystickAxis/MouseWheel, pressed=0 for value < 0.5, pressed=1 otherwise.
    typedef std::function<void(Boolean)> SwitchKeybindHandler;

    // void handle(Float strength), event called on change, strength is useful for joystick keybinds.
    // strength is always in range -1 - 1
    // For non-JoystickAxis/MouseWheel: press -> strength=1; release -> strength=0
    // For key pairs: none pressed / both pressed -> strength=0, minus pressed -> strength=-1, plus pressed -> strength=1
    typedef std::function<void(Float)> StrengthKeybindHandler; // for joystick

    // TODO: add API for mouse moves

    // Uses last file name if name not given. The default is 'keybinds.json'
    // The fileName is relative to 'config' dir.
    bool load(String fileName = "");

    // Last file name, or 'keybinds.json' is used by default.
    bool save(String fileName = "");

    void addTrigger(String name, Input defaultInput, TriggerKeybindHandler handler);
    void addSwitch(String name, Input defaultInput, SwitchKeybindHandler handler);
    void addStrength(String name, Input defaultInput, StrengthKeybindHandler handler);

    void setTrigger(String name, Input);
    void setSwitch(String name, Input);
    void setStrength(String name, Input);

    static void hook(UniquePtr<KeybindManager>&& manager, ComponentBase& loop);

    // TODO: Allow iteration for 'controls' GUI

    virtual void onKeyPress(sf::Event::KeyEvent&) override;
    virtual void onKeyRelease(sf::Event::KeyEvent&) override;
    virtual void onMouseWheelScroll(sf::Event::MouseWheelScrollEvent&) override;
    virtual void onMouseButtonPress(sf::Event::MouseButtonEvent&) override;
    virtual void onMouseButtonRelease(sf::Event::MouseButtonEvent&) override;
    virtual void onJoystickButtonPress(sf::Event::JoystickButtonEvent&) override;
    virtual void onJoystickButtonRelease(sf::Event::JoystickButtonEvent&) override;
    virtual void onJoystickMove(sf::Event::JoystickMoveEvent&) override;

private:
    struct TriggerKB { Input input; TriggerKeybindHandler handler; };
    struct SwitchKB { Input input; SwitchKeybindHandler handler; };
    struct StrengthKB { Input input; StrengthKeybindHandler handler; };

    Vector<const TriggerKB*> findAllTriggerKBs(Input) const;
    Vector<const SwitchKB*> findAllSwitchKBs(Input) const;
    Vector<const StrengthKB*> findAllStrengthKBs(Input) const;

    void callAllTriggerKBs(Input);
    void callAllSwitchKBs(Input, Boolean value);
    void callAllStrengthKBs(Input, Float value);
    void callAllKeyPairs(bool release, sf::Keyboard::Key key);

    String m_lastFileName = "keybinds.json";
    StringMap<TriggerKB> m_triggerKBs;
    StringMap<SwitchKB> m_switchKBs;
    StringMap<StrengthKB> m_strengthKBs;
};

}
