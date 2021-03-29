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
        MouseWheel
        // TODO: mouse move
    } type;

    union Value
    {
        sf::Keyboard::Key key; // For type=Keyboard

        struct
        {
            int id = 0;
            union
            {
                sf::Joystick::Axis axis; // For type=JoystickAxis
                int button; // For type=JoystickButton
            };
        } joystick;

        sf::Mouse::Button mouseButton; // For type=MouseButton
        sf::Mouse::Wheel mouseWheel; // For type=MouseWheel
    } value;


    Input(sf::Keyboard::Key key)
        : type(Keyboard), value({.key = key}) {}

    Input(int id, sf::Joystick::Axis axis)
        : type(JoystickAxis), value({.joystick = {.id = id, .axis = axis}}) {}

    Input(int id, int button)
        : type(JoystickButton), value({.joystick = {.id = id, .button = button}}) {}

    Input(sf::Mouse::Button button)
        : type(MouseButton), value({.mouseButton = button}) {}

    Input(sf::Mouse::Wheel wheel)
        : type(MouseWheel), value({.mouseWheel = wheel}) {}

    bool operator==(const Input& other) const;
    bool operator!=(const Input& other) const { return !(*this == other); }
};

class EventLoop;

class KeybindManager : public SystemEventHandler
{
public:
    // void handle(), event called on press.
    // For JoystickAxis/MouseWheel, called on any 'up' move (value > 0).
    // NOTE: this allows to bind scroll up to space :^)
    typedef std::function<void()> TriggerKeybindHandler;

    // void handle(Boolean released), event called on press(released=false), and release(released=true)
    // For JoystickAxis/MouseWheel, released=0 for value < 0.5, released=1 otherwise.
    typedef std::function<void(Boolean)> SwitchKeybindHandler;

    // void handle(Float strength), event called on change, strength is useful for joystick keybinds.
    // strength is always in range -1 - 1
    // For non-JoystickAxis/MouseWheel: press -> strength=1; release -> strength=0
    typedef std::function<void(Float)> StrengthKeybindHandler; // for joystick

    // TODO: add API for mouse moves
    // TODO: API for common key groups (WASD, arrows, ...)

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

    static void hook(SharedPtr<KeybindManager> manager, EventLoop& loop);

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

    String m_lastFileName = "keybinds.json";
    StringMap<TriggerKB> m_triggerKBs;
    StringMap<SwitchKB> m_switchKBs;
    StringMap<StrengthKB> m_strengthKBs;
};

}
