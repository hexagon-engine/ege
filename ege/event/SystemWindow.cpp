/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#include "SystemWindow.h"

#include <ege/debug/Logger.h>
#include <ege/event/SystemEvent.h>
#include <GL/glew.h>

namespace EGE
{

void SFMLSystemWindow::callEvents(EventLoop* loop, SFMLSystemWindow::WaitForEvents wait)
{
    if(wait == SFMLSystemWindow::WaitForEvents::Yes)
    {
        sf::Event event;
        ASSERT(waitEvent(event));
        SystemEvent sysEvent(event);
        ASSERT(loop->fireEvent(sysEvent) == EventResult::Success);
    }
    else
    {
        sf::Event event;
        while(pollEvent(event))
        {
            SystemEvent sysEvent(event);
            ASSERT(loop->fireEvent(sysEvent) == EventResult::Success);
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
    }
    m_glExtensions = true;
}

}
