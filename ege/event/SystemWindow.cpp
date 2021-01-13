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

#include "SystemWindow.h"

#include <ege/debug/Logger.h>
#include <ege/event/SystemEvent.h>
#include <GL/glew.h>

namespace EGE
{

void SFMLSystemWindow::callEvents(EventLoop& loop, SFMLSystemWindow::WaitForEvents wait)
{
    if(wait == SFMLSystemWindow::WaitForEvents::Yes)
    {
        sf::Event event;
        ASSERT(waitEvent(event));
        ASSERT(loop.fire<SystemEvent>(event) == EventResult::Success);
    }
    else
    {
        sf::Event event;
        while(pollEvent(event))
        {
            SystemEvent sysEvent(event);
            ASSERT(loop.fire<SystemEvent>(event) == EventResult::Success);
        }
    }
}

void SFMLSystemWindow::initialize()
{
    sf::RenderTarget::initialize();

    // Initialize GLEW
    GLenum rc = glewInit();
    if(rc != GLEW_OK)
    {
        err(LogLevel::Crash) << "GLEW initialization failed: " << glewGetErrorString(rc);
        return;
    }
    m_glExtensions = true;
}

}
