/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#include <testsuite/Tests.h>
#include <ege/loop/GameLoop.h>
#include <ege/sysevent/SystemEvent.h>
#include <ege/syswindow/DefaultSystemEventHandler.h>
#include <ege/syswindow/SystemWindow.h>

class MyGameLoop : public EGE::GameLoop
{
    std::shared_ptr<EGE::SFMLSystemWindow> m_window;
public:
    virtual EGE::EventResult onLoad();
    virtual void onTick(long long tickCount)
    {
        //DEBUG_PRINT("onTick");
        m_window->callEvents(this);
        if(!m_window->isOpen())
            exit();
        m_window->clear();
        m_window->display();
    }
    virtual void onExit(int exitCode) { (void)exitCode; }
    virtual EGE::EventResult onFinish(int exitCode) { (void)exitCode; return EGE::EventResult::Success; }
};

class MySystemEventHandler : public EGE::DefaultSystemEventHandler
{
public:
    MySystemEventHandler(std::weak_ptr<EGE::SFMLSystemWindow> window)
    : EGE::DefaultSystemEventHandler(window)
    {}

    virtual void onMouseEnter()
    {
        DEBUG_PRINT("onMouseEnter");
    }
    virtual void onMouseLeave()
    {
        DEBUG_PRINT("onMouseLeave");
    }
};

EGE::EventResult MyGameLoop::onLoad()
{
    m_window = std::make_shared<EGE::SFMLSystemWindow>();
    addEventHandler(EGE::SystemEvent::getTypeStatic(), new MySystemEventHandler(m_window));
    m_window->create(sf::VideoMode(256, 256), "EGE Test");
    return EGE::EventResult::Success;
}

MyGameLoop loop;

TESTCASE(createWindow)
{
    loop.run();
}

RUN_TESTS(syswindow)
