#include <ege/core.h>

#include "TickEvent.h"

int main()
{
    EGE::EventLoop loop;
    loop.events<TickEvent>().add([](TickEvent&){ return EGE::EventResult::Success; });
    loop.events<TickEvent>().add([](TickEvent&){ return EGE::EventResult::Failure; });
    loop.events<TickEvent>().add([](TickEvent&){ return EGE::EventResult::Success; });
    for(size_t s = 0; s < 1024000; s++)
    {
        loop.fire<TickEvent>(s);
    }
    return 0;
}
