/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#include "SystemWindow.h"

#include <ege/event/SystemEvent.h>

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

}
