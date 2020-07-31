#include "SystemWindow.h"

#include <sysevent/SystemEvent.h>

namespace EGE
{

void SFMLSystemWindow::callEvents(GameLoop* loop, SFMLSystemWindow::WaitForEvents wait)
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
