#include <testsuite/Tests.h>
#include <loop/GameLoop.h>
#include <sysevent/SystemEventHandler.h>

class MyGameLoop : public EGE::GameLoop
{
public:
    virtual EGE::EventResult onLoad() { return EGE::EventResult::Success; }
    virtual void onTick(long long tickCount)
    {
        DEBUG_PRINT("onTick");
        sf::Event event;
        switch(tickCount)
        {
            case 1:
            {
                event.type = sf::Event::MouseButtonReleased;
                event.mouseButton.button = sf::Mouse::XButton1;
                EGE::SystemEvent sysevent(event);
                fireEvent(EGE::SystemEvent::getTypeStatic(), sysevent);
            } break;
            case 2:
            {
                event.type = sf::Event::MouseMoved;
                event.mouseMove.x = 123;
                event.mouseMove.y = 321;
                EGE::SystemEvent sysevent(event);
                fireEvent(EGE::SystemEvent::getTypeStatic(), sysevent);
            } break;
            case 3:
            {
                event.type = sf::Event::MouseEntered;
                EGE::SystemEvent sysevent(event);
                fireEvent(EGE::SystemEvent::getTypeStatic(), sysevent);
            } break;
            case 4:
            {
                exit();
            } break;
        }
    }
    virtual void onExit(int exitCode) { (void)exitCode; }
    virtual EGE::EventResult onFinish(int exitCode) { (void)exitCode; return EGE::EventResult::Success; }
};

MyGameLoop loop;

class MySystemEventHandler : public EGE::SystemEventHandler
{
public:
    virtual void onMouseButtonRelease(sf::Event::MouseButtonEvent& event)
    {
        EXPECT_EQUAL(event.button, sf::Mouse::XButton1);
        DEBUG_PRINT("onMouseButtonRelease");
    }
    virtual void onMouseMove(sf::Event::MouseMoveEvent& event)
    {
        EXPECT_EQUAL(event.x, 123);
        EXPECT_EQUAL(event.y, 321);
        DEBUG_PRINT("onMouseMove");
    }
    virtual void onMouseEnter()
    {
        DEBUG_PRINT("onMouseEnter");
    }
};

TESTCASE(event)
{
    EXPECT_EQUAL(EGE::EventCast<EGE::Event>(new EGE::SystemEvent(sf::Event()))->getType(), "EGE::SystemEvent");
    loop.addEventHandler(EGE::SystemEvent::getTypeStatic(), std::shared_ptr<EGE::EventHandler>(new MySystemEventHandler));
    loop.run();
}

RUN_TESTS(sysevent)
