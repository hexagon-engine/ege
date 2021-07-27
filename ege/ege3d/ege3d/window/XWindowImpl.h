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

#include <ege3d/window/WindowImpl.h>
#include <GL/glx.h>
#include <X11/Xlib.h>

namespace EGE3d
{

namespace Internal
{

class XWindowImpl final : public WindowImpl
{
public:
    XWindowImpl(Window* owner)
    : WindowImpl(owner) {}

    virtual WindowHandle create(size_t sx, size_t sy, std::string title, WindowSettings settings) override;
    virtual void close() override;
    virtual bool dispatchEvent(bool wait) override;
    virtual void display() override;

private:
    virtual void handleEvent(XEvent& event);
    Atom getAtom(std::string name);
    Atom getOrCreateAtom(std::string name);

    Display* m_display = nullptr;
    int m_screen = -1;
    ::Window m_window;
    GC m_gc;
    GLXContext m_glxContext;
    bool m_needRedraw = false;
    WindowSettings m_settings;
};

}

}
