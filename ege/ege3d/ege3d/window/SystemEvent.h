/*
    EGE3d - 3D rendering engine for Hexagon

    Copyright (c) 2020 Hexagon Engine

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.
*/

#pragma once

#include <ege3d/window/Keyboard.h>
#include <ege3d/window/Mouse.h>
#include <inttypes.h>

namespace EGE3d
{

class Window;

enum class EventType
{
    EInvalid,
    EExpose,
    EClose,
    EDestroy,
    EMouseMove,
    EMouseWheel,
    EMouseButtonPress,
    EMouseButtonRelease,
    EKeyPress,
    ETextEnter,
    EKeyRelease,

    // keep last
    __Count
};

// SystemEvent
class SystemEvent
{
public:
    SystemEvent(EventType type, Window& window)
    : m_type(type), m_window(window) {}

    Window& getWindow() const { return m_window; }
    EventType getType() const { return m_type; }

private:
    EventType m_type;
    Window& m_window;
};

// MouseMoveEvent
class MouseMoveEvent : public SystemEvent
{
public:
    MouseMoveEvent(EventType type, Window& window, int x, int y)
    : SystemEvent(type, window), m_x(x), m_y(y) {}

    int getX() const { return m_x; }
    int getY() const { return m_y; }

private:
    int m_x;
    int m_y;
};

// MouseButtonEvent
class MouseButtonEvent : public SystemEvent
{
public:
    MouseButtonEvent(EventType type, Window& window, Mouse::Button button)
    : SystemEvent(type, window), m_button(button) {}

    Mouse::Button getButton() const { return m_button; }

private:
    Mouse::Button m_button;
};

// MouseWheelEvent
class MouseWheelEvent : public SystemEvent
{
public:
    MouseWheelEvent(EventType type, Window& window, int delta)
    : SystemEvent(type, window), m_delta(delta) {}

    int getDelta() const { return m_delta; }

private:
    int m_delta;
};

// KeyEvent
class KeyEvent : public SystemEvent
{
public:
    KeyEvent(EventType type, Window& window, Keyboard::Key key, bool alt, bool shift, bool ctrl, bool super)
    : SystemEvent(type, window), m_key(key), m_alt(alt), m_shift(shift), m_ctrl(ctrl), m_super(super) {}

    Keyboard::Key getKey() const { return m_key; }
    bool isAlt() const { return m_alt; }
    bool isShift() const { return m_shift; }
    bool isCtrl() const { return m_ctrl; }
    bool isSuper() const { return m_super; }

private:
    Keyboard::Key m_key;
    bool m_alt;
    bool m_shift;
    bool m_ctrl;
    bool m_super;
};

// TextEvent
class TextEvent : public SystemEvent
{
public:
    TextEvent(EventType type, Window& window, uint32_t code)
    : SystemEvent(type, window), m_code(code) {}

    uint32_t getCode() const { return m_code; }

private:
    uint32_t m_code;
};

}
