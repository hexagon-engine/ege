#include <ege/core.h>

#include "TickEvent.h"

int main()
{
    EGE::EventLoop loop;
    for(size_t s = 0; s < 1024000; s++)
    {
        loop.fire<TickEvent>(s);
    }
    return 0;
}
