#include <testsuite/Tests.h>
#include <ege/loop/GameLoop.h>
#include <ege/loop/TimerEvent.h>
#include <iostream>

using EGE::Time;
using EGE::Timer;

class MyGameLoop : public EGE::GameLoop
{
public:
    virtual EGE::EventResult onLoad() override;
    virtual void onTick(long long tickCount) override;
    virtual void onExit(int exitCode) override;
    virtual EGE::EventResult onFinish(int exitCode) override;
};

struct TickEvent : public EGE::Event
{
    long long m_tickCount;

    TickEvent(long long tickCount)
        : m_tickCount(tickCount)
    {}

    EGE_EVENT("TickEvent");
};

EGE::EventResult MyGameLoop::onLoad()
{
    DEBUG_PRINT("onLoad");
    return EGE::EventResult::Success;
}
void MyGameLoop::onTick(long long tickCount)
{
    DEBUG_PRINT("onTick");

    TickEvent event(tickCount);
    auto success = fireEvent(event);

    if(tickCount == 2)
        EXPECT(event.isCanceled());

    if(tickCount == 3)
        exit(1);
}
void MyGameLoop::onExit(int exitCode)
{
    DEBUG_PRINT("onExit");
}
EGE::EventResult MyGameLoop::onFinish(int exitCode)
{
    DEBUG_PRINT("onFinish");
    return EGE::EventResult::Failure;
}

EGE::EventResult eventTest(TickEvent& event)
{
    DEBUG_PRINT("eventTest");
    if(event.m_tickCount == 2)
        event.cancel();
    return EGE::EventResult::Success;
}
EGE::EventResult eventTest2(TickEvent& event)
{
    DEBUG_PRINT("eventTest2");
    return EGE::EventResult::Failure;
}

TESTCASE(gameLoop)
{
    MyGameLoop gameLoop;
    gameLoop.addEventHandler("TickEvent", new EGE::SimpleEventHandler<TickEvent>(eventTest));
    gameLoop.addEventHandler("TickEvent", new EGE::SimpleEventHandler<TickEvent>(eventTest2));
    int rc = gameLoop.run();
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
    void onTick(long long tickCount);
};

void MyGameLoop2::onTick(long long tickCount)
{
    //DEBUG_PRINT("onTick :2");

    GameLoop::onUpdate();
}

TESTCASE(time)
{
    MyGameLoop2 loop;
    auto callback = [](std::string name, EGE::Timer* timer) {
                        std::cerr << name << std::endl;
                    };

    loop.addTimer("display-sec-count", &(*new Timer(&loop, Timer::Mode::Infinite, Time(1.f, Time::Unit::Seconds)))
                    .setCallback([](std::string name, Timer* timer) { std::cerr << timer->getIterationCount() << " seconds" << std::endl; }), EGE::GameLoop::TimerImmediateStart::Yes);
    loop.addTimer("single-shot 5s", &(*new Timer(&loop, Timer::Mode::Limited, Time(5.f, Time::Unit::Seconds)))
                    .setCallback(callback), EGE::GameLoop::TimerImmediateStart::Yes);
    loop.addTimer("5-shots-ticks 5s", &(*new Timer(&loop, Timer::Mode::Limited, Time(5.f, Time::Unit::Ticks)))
                    .setCallback(callback).setRemainingIterationCount(5), EGE::GameLoop::TimerImmediateStart::Yes);
    loop.addTimer("infinite 2s", &(*new Timer(&loop, Timer::Mode::Infinite, Time(0.5f, Time::Unit::Seconds)))
                    .setCallback(callback), EGE::GameLoop::TimerImmediateStart::Yes);
    loop.addTimer("stop-test", &(*new Timer(&loop, Timer::Mode::Limited, Time(10.f, Time::Unit::Seconds)))
                    .setCallback([&loop](std::string name, Timer* timer) { loop.getTimers("infinite 2s")[0].lock()->stop(); }), EGE::GameLoop::TimerImmediateStart::Yes);
    loop.addTimer("restart-test", &(*new Timer(&loop, Timer::Mode::Limited, Time(12.f, Time::Unit::Seconds)))
                    .setCallback([&loop](std::string name, Timer* timer) { loop.getTimers("infinite 2s")[0].lock()->start(); }), EGE::GameLoop::TimerImmediateStart::Yes);
    loop.addTimer("remove-test", &(*new Timer(&loop, Timer::Mode::Limited, Time(14.f, Time::Unit::Seconds)))
                    .setCallback([&loop](std::string name, Timer* timer) { loop.removeTimer("infinite 2s"); }), EGE::GameLoop::TimerImmediateStart::Yes);
    loop.addTimer("close", &(*new Timer(&loop, Timer::Mode::Limited, Time(15.f, Time::Unit::Seconds)))
                    .setCallback([&loop](std::string name, Timer* timer) { loop.exit(); }), EGE::GameLoop::TimerImmediateStart::Yes);
    loop.addTimer("test-events", &(*new Timer(&loop, Timer::Mode::Infinite, Time(1.f, Time::Unit::Seconds))), EGE::GameLoop::TimerImmediateStart::Yes);

    loop.run();
}

RUN_TESTS(loop)
