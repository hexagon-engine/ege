#include <testsuite/Tests.h>
#include <ege/loop/EventLoop.h>
#include <ege/loop/TimerEvent.h>
#include <iostream>

using EGE::Time;
using EGE::Timer;

class MyGameLoop : public EGE::EventLoop
{
public:
    virtual void onUpdate(long long tickCount);
};

struct TickEvent : public EGE::Event
{
    long long m_tickCount;

    TickEvent(long long tickCount)
        : m_tickCount(tickCount)
    {}

    EGE_EVENT("TickEvent");
};

void MyGameLoop::onUpdate(long long tickCount)
{
    EGE::EventLoop::onUpdate();
    DEBUG_PRINT("onTick");

    TickEvent event(tickCount);
    events<TickEvent>().fire(event);

    if(tickCount == 2)
        EXPECT(event.isCanceled());

    if(tickCount == 3)
        exit(0);
}

EGE::EventResult eventTest(TickEvent& event)
{
    DEBUG_PRINT("eventTest");
    if(event.m_tickCount == 2)
        event.cancel();
    return EGE::EventResult::Success;
}
EGE::EventResult eventTest2(TickEvent&)
{
    DEBUG_PRINT("eventTest2");
    return EGE::EventResult::Failure;
}

TESTCASE(gameLoop)
{
    MyGameLoop gameLoop;
    gameLoop.events<TickEvent>().add(eventTest);
    gameLoop.events<TickEvent>().add(eventTest2);

    return gameLoop.run();
}

EGE::EventResult timerEventTest(EGE::TimerEvent& event)
{
    std::cerr << "-- TimerEventTest: " << event.getTimer()->getName() << std::endl;
    switch(event.getEventType())
    {
    case EGE::TimerEvent::Finish:
        DEBUG_PRINT("TimerEvent Finish");
        break;
    case EGE::TimerEvent::Start:
        DEBUG_PRINT("TimerEvent Start");
        break;
    case EGE::TimerEvent::Tick:
        DEBUG_PRINT("TimerEvent Tick");
        break;
    }

    return EGE::EventResult::Success;
}

class MyGameLoop2 : public MyGameLoop
{
public:
    void onUpdate(long long tickCounter);
};

void MyGameLoop2::onUpdate(long long tickCounter)
{
    //DEBUG_PRINT("onTick :2");

    MyGameLoop::onUpdate(tickCounter);
}

TESTCASE(time)
{
    MyGameLoop2 loop;
    auto callback = [](std::string name, EGE::Timer*) {
                        std::cerr << name << std::endl;
                    };

    loop.addTimer("display-sec-count", make<Timer>(loop, Timer::Mode::Infinite, 1.0, [](std::string, Timer* timer) {
        std::cerr << timer->getIterationCount() << " seconds" << std::endl;
    }));

    loop.addTimer("single-shot 5s", make<Timer>(loop, Timer::Mode::Limited, 5.0, callback));

    auto timer = make<Timer>(loop, Timer::Mode::Limited, 5.0, callback);
    timer->setRemainingIterationCount(5);
    loop.addTimer("5-shots-ticks 5s", timer);

    loop.addTimer("infinite 2s", make<Timer>(loop, Timer::Mode::Infinite, 0.5, callback));

    loop.addTimer("stop-test", make<Timer>(loop, Timer::Mode::Limited, 10.0, [&loop](std::string, Timer*) {
        loop.getTimers("infinite 2s")[0].lock()->stop();
    }));
    loop.addTimer("restart-test", make<Timer>(loop, Timer::Mode::Limited, 12.0, [&loop](std::string, Timer*) {
        loop.getTimers("infinite 2s")[0].lock()->start();
    }));
    loop.addTimer("remove-test", make<Timer>(loop, Timer::Mode::Limited, 14.0, [&loop](std::string, Timer*) {
        loop.removeTimer("infinite 2s");
    }));
    loop.addTimer("close", make<Timer>(loop, Timer::Mode::Limited, 15.0, [&loop](std::string, Timer*) {
        loop.exit();
    }));
    loop.addTimer("test-events", make<Timer>(loop, Timer::Mode::Infinite, 1.0));

    return loop.run();
}

RUN_TESTS(loop)
