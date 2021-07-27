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

#include <ege3d/window/SystemEvent.h>
#include <ege3d/window/WindowImpl.h>
#include <ege3d/window/WindowSettings.h>

#include <functional>
#include <memory>
#include <string>

#define EGE3D_WINDOW_DEFAULT 0

namespace EGE3d
{

class Window
{
public:
    Window();
    virtual ~Window();

    bool create(size_t sx, size_t sy, std::string title, WindowSettings settings = {});
    void close();
    WindowHandle getSystemHandle();
    bool dispatchEvent(bool wait = false);
    void display();

    bool isOpen();
    void dispatchAllEvents(bool wait = false);

    void setEventHandler(std::function<void(const SystemEvent&)> handler) { m_eventHandler = handler; }

    virtual void onEvent(const SystemEvent& event);

private:
    std::unique_ptr<Internal::WindowImpl> m_impl;
    WindowHandle m_systemHandle = 0;

    std::function<void(const SystemEvent&)> m_eventHandler;
};

}
