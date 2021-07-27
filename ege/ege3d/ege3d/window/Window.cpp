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

#include "Window.h"

#include <ege3d/window/WindowImpl.h>
#include <iostream>

namespace EGE3d
{

namespace Internal
{

// It is declared here and should be defined by system-specific implementation.
std::unique_ptr<WindowImpl> make(Window*);

}

Window::Window()
: m_impl(Internal::WindowImpl::make(this))
{

}

Window::~Window()
{
    close();
}

bool Window::create(size_t sx, size_t sy, std::string title, WindowSettings settings)
{
    WindowHandle handle = m_impl->create(sx, sy, title, settings);
    if(!handle)
        return false;
    m_systemHandle = handle;
    return true;
}

void Window::close()
{
    if(m_systemHandle)
    {
        m_impl->close();
        m_systemHandle = 0;
    }
}

void Window::display()
{
    m_impl->display();
}

bool Window::dispatchEvent(bool wait)
{
    return m_impl->dispatchEvent(wait);
}

bool Window::isOpen()
{
    return m_systemHandle != 0;
}

void Window::dispatchAllEvents(bool wait)
{
    while(isOpen() && m_impl->dispatchEvent(wait))
        ;
}

void Window::onEvent(const SystemEvent& event)
{
    std::cout << "event :)" << std::endl;
    if(m_eventHandler)
        m_eventHandler(event);
}

}
