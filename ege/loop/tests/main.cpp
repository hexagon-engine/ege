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
    fireEvent(event);

    if(tickCount == 2)
        EXPECT(event.isCanceled());

    if(tickCount == 3)
        running = false;
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
    gameLoop.addEventHandler("TickEvent", new EGE::SimpleEventHandler<TickEvent>(eventTest));
    gameLoop.addEventHandler("TickEvent", new EGE::SimpleEventHandler<TickEvent>(eventTest2));

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

    loop.addTimer("display-sec-count", &(*new Timer(&loop, Timer::Mode::Infinite, Time(1.f, Time::Unit::Seconds)))
                    .setCallback([](std::string, Timer* timer) { std::cerr << timer->getIterationCount() << " seconds" << std::endl; }), EGE::EventLoop::TimerImmediateStart::Yes);
    loop.addTimer("single-shot 5s", &(*new Timer(&loop, Timer::Mode::Limited, Time(5.f, Time::Unit::Seconds)))
                    .setCallback(callback), EGE::EventLoop::TimerImmediateStart::Yes);
    loop.addTimer("5-shots-ticks 5s", &(*new Timer(&loop, Timer::Mode::Limited, Time(5.f, Time::Unit::Ticks)))
                    .setCallback(callback).setRemainingIterationCount(5), EGE::EventLoop::TimerImmediateStart::Yes);
    loop.addTimer("infinite 2s", &(*new Timer(&loop, Timer::Mode::Infinite, Time(0.5f, Time::Unit::Seconds)))
                    .setCallback(callback), EGE::EventLoop::TimerImmediateStart::Yes);
    loop.addTimer("stop-test", &(*new Timer(&loop, Timer::Mode::Limited, Time(10.f, Time::Unit::Seconds)))
                    .setCallback([&loop](std::string, Timer*) { loop.getTimers("infinite 2s")[0].lock()->stop(); }), EGE::EventLoop::TimerImmediateStart::Yes);
    loop.addTimer("restart-test", &(*new Timer(&loop, Timer::Mode::Limited, Time(12.f, Time::Unit::Seconds)))
                    .setCallback([&loop](std::string, Timer*) { loop.getTimers("infinite 2s")[0].lock()->start(); }), EGE::EventLoop::TimerImmediateStart::Yes);
    loop.addTimer("remove-test", &(*new Timer(&loop, Timer::Mode::Limited, Time(14.f, Time::Unit::Seconds)))
                    .setCallback([&loop](std::string, Timer*) { loop.removeTimer("infinite 2s"); }), EGE::EventLoop::TimerImmediateStart::Yes);
    loop.addTimer("close", &(*new Timer(&loop, Timer::Mode::Limited, Time(15.f, Time::Unit::Seconds)))
                    .setCallback([&loop](std::string, Timer*) { loop.running = false; }), EGE::EventLoop::TimerImmediateStart::Yes);
    loop.addTimer("test-events", &(*new Timer(&loop, Timer::Mode::Infinite, Time(1.f, Time::Unit::Seconds))), EGE::EventLoop::TimerImmediateStart::Yes);

    while(loop.running)
    {
        loop.onUpdate(loop.m_tickCounter);
        loop.m_tickCounter++;
    }

    return 0;
}

RUN_TESTS(loop)
