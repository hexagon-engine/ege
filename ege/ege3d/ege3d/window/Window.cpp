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
#include "ege3d/window/SystemEvent.h"

#include <ege3d/window/WindowImpl.h>
#include <iostream>

namespace EGE3d
{

namespace Internal
{

// It is declared here and should be defined by system-specific implementation.
std::unique_ptr<WindowImpl> make(Window*);
Window* currentWindow = nullptr;

}

Window::Window()
: m_impl(Internal::WindowImpl::make(this))
{

}

Window::~Window()
{
    close();
    if(this == Internal::currentWindow)
        Internal::currentWindow = nullptr;
}

bool Window::create(unsigned sx, unsigned sy, std::string title, WindowSettings settings)
{
    WindowHandle handle = m_impl->create(sx, sy, title, settings);
    if(!handle)
        return false;
    m_systemHandle = handle;
    m_size = {sx, sy};
    setCurrent();
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

EGE::Optional<SystemEvent> Window::nextEvent(bool wait)
{
    if(m_pendingEvents.empty())
    {
        if(!m_impl->dispatchEvent(wait))
            return {};
    }
    auto event = std::move(m_pendingEvents.back());
    m_pendingEvents.pop_back();
    return event;
}

bool Window::isOpen() const
{
    return m_systemHandle != 0;
}

void Window::pushEvent(SystemEvent const& event)
{
    std::cout << "event :)" << std::endl;
    m_pendingEvents.push_back(std::move(event));

    if(event.getEventType() == SystemEventType::EResize)
    {
        auto& resizeEvent = static_cast<ResizeEvent const&>(event);
        m_size = resizeEvent.getSize();
    }
}

bool Window::isCurrent() const
{
    return this == Internal::currentWindow;
}

void Window::setCurrent()
{
    Internal::currentWindow = this;
    m_impl->setCurrent();
}

}
