#include <testsuite/Tests.h>
#include <loop/GameLoop.h>

#undef DEBUG_PRINT
#define DEBUG_PRINT(x)

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
    auto success = fireEvent("TickEvent", event);

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
    gameLoop.addEventHandler("TickEvent", std::shared_ptr<EGE::EventHandler>(new EGE::SimpleEventHandler<TickEvent>(eventTest)));
    gameLoop.addEventHandler("TickEvent", std::shared_ptr<EGE::EventHandler>(new EGE::SimpleEventHandler<TickEvent>(eventTest2)));
    int rc = gameLoop.run();
    EXPECT_EQUAL(rc, 0x0002);
}

class MyGameLoop2 : public MyGameLoop
{
public:
    virtual void onTick(long long tickCount);
};

void MyGameLoop2::onTick(long long tickCount)
{
    MyGameLoop::onTick(tickCount);
    if(tickCount == 1)
        addEventHandler("TickEvent", std::shared_ptr<EGE::EventHandler>(new EGE::SimpleEventHandler<TickEvent>(eventTest2)));
}

TESTCASE(gameLoopDynamicEventHandlers)
{
    MyGameLoop2 gameLoop;
    gameLoop.addEventHandler("TickEvent", std::shared_ptr<EGE::EventHandler>(new EGE::SimpleEventHandler<TickEvent>(eventTest)));
    int rc = gameLoop.run();
}

TESTCASE(gameLoopMultipleEventTypes)
{
    MyGameLoop gameLoop;
    gameLoop.addEventHandler("TickEvent", std::shared_ptr<EGE::EventHandler>(new EGE::SimpleEventHandler<TickEvent>(eventTest)));
    gameLoop.addEventHandler("TickEvent", std::shared_ptr<EGE::EventHandler>(new EGE::SimpleEventHandler<TickEvent>(eventTest2)));
    gameLoop.addEventHandler("TickEvent2", std::shared_ptr<EGE::EventHandler>(new EGE::SimpleEventHandler<TickEvent>(eventTest)));
    gameLoop.addEventHandler("TickEvent2", std::shared_ptr<EGE::EventHandler>(new EGE::SimpleEventHandler<TickEvent>(eventTest2)));
    int rc = gameLoop.run();
}

RUN_TESTS(loop)
