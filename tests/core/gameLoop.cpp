#include <ege/core.h>
#include <ege/debug.h>
#include <tests/Tests.h>

#include "TickEvent.h"

class MyGameLoop : public EGE::MainLoop
{
public:
    virtual void onUpdate() override;
};

void MyGameLoop::onUpdate()
{
    EGE::EventLoop::onUpdate();
    ege_log.info() << "onTick";
    auto tickCount = getTickCount();

    TickEvent event(tickCount);
    events<TickEvent>().fire(event);

    if(tickCount == 2)
        EXPECT(event.isCanceled());

    if(tickCount == 3)
        exit(0);
}

EGE::EventResult eventTest(TickEvent& event)
{
    ege_log.info() << "eventTest";
    if(event.m_tickCount == 2)
        event.cancel();
    return EGE::EventResult::Success;
}
EGE::EventResult eventTest2(TickEvent&)
{
    ege_log.info() << "eventTest2";
    return EGE::EventResult::Failure;
}

int main()
{
    MyGameLoop gameLoop;
    gameLoop.events<TickEvent>().add(eventTest);
    gameLoop.events<TickEvent>().add(eventTest2);

    return gameLoop.run();
}
