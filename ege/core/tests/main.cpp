#include <testsuite/Tests.h>
#include <ege/core/DataManager.h>
#include <ege/core/MainLoop.h>
#include <ege/core/TimerEvent.h>
#include <ege/debug/Logger.h>
#include <ege/util/ObjectSerializers.h>
#include <ege/util/Types.h>
#include <iostream>

using EGE::Timer;

class MyGameLoop : public EGE::MainLoop
{
public:
    virtual void onTick() override;
};

struct TickEvent : public EGE::Event
{
    long long m_tickCount;

    TickEvent(long long tickCount)
        : m_tickCount(tickCount)
    {}

    EGE_EVENT("TickEvent");
};

void MyGameLoop::onTick()
{
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

TESTCASE(gameLoop)
{
    MyGameLoop gameLoop;
    gameLoop.events<TickEvent>().add(eventTest);
    gameLoop.events<TickEvent>().add(eventTest2);

    return gameLoop.run();
}

TESTCASE(_eventPerfTest)
{
    EGE::ComponentBase loop;
    for(size_t s = 0; s < 1024000; s++)
    {
        loop.fire<TickEvent>(s);
    }
    return 0;
}

TESTCASE(_eventPerfTest_Handler)
{
    EGE::ComponentBase loop;
    loop.events<TickEvent>().add([](TickEvent&){ return EGE::EventResult::Success; });
    for(size_t s = 0; s < 1024000; s++)
    {
        loop.fire<TickEvent>(s);
    }
    return 0;
}

TESTCASE(_eventPerfTest_3Handlers)
{
    EGE::ComponentBase loop;
    loop.events<TickEvent>().add([](TickEvent&){ return EGE::EventResult::Success; });
    loop.events<TickEvent>().add([](TickEvent&){ return EGE::EventResult::Failure; });
    loop.events<TickEvent>().add([](TickEvent&){ return EGE::EventResult::Success; });
    for(size_t s = 0; s < 1024000; s++)
    {
        loop.fire<TickEvent>(s);
    }
    return 0;
}

TESTCASE(time)
{
    EGE::MainLoop loop;
    auto callback = [](std::string name, EGE::Timer*) {
        std::cerr << name << std::endl;
    };

    // TODO: Fix deadlock on accessing timers from timer handlers
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

    loop.events<EGE::TimerStartEvent>().add([](EGE::TimerStartEvent& event) {
        std::cerr << "Timer started: " << event.timer.getName() << std::endl;
        return EGE::EventResult::Success;
    });
    loop.events<EGE::TimerTickEvent>().add([](EGE::TimerTickEvent& event) {
        std::cerr << "Timer tick: " << event.timer.getName() << std::endl;
        return EGE::EventResult::Success;
    });
    loop.events<EGE::TimerFinishEvent>().add([](EGE::TimerFinishEvent& event) {
        std::cerr << "Timer finished: " << event.timer.getName() << std::endl;
        return EGE::EventResult::Success;
    });

    return loop.run();
}

TESTCASE(dataManager)
{
    {
        EGE::DataManager manager("res/config.json");
        EXPECT(!manager.error());
        EXPECT_EQUAL(manager.getValue("test").asBoolean().value(), true);
        manager.setValue("test2", EGE::Serializers::object(true));
        EXPECT(manager.save());
    }
    return 0;
}

RUN_TESTS(loop)
