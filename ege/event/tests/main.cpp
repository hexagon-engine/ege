/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#include <testsuite/Tests.h>
#include <ege/loop/EventLoop.h>
#include <ege/event/SystemEvent.h>
#include <ege/event/DefaultSystemEventHandler.h>
#include <ege/event/SystemWindow.h>

class MyGameLoop : public EGE::EventLoop
{
    std::shared_ptr<EGE::SFMLSystemWindow> m_window;
public:
    virtual EGE::EventResult onLoad();
    virtual void onTick(long long)
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

    int run();
    void exit(int exitCode = 0);

private:
    bool m_running = true;
    int m_exitCode = 0;
    long long m_tickCounter = 0;
};

int MyGameLoop::run()
{
    auto result = onLoad();
    if(result == EGE::EventResult::Failure)
    {
        std::cerr << "0001 EGE/loop: load failed" << std::endl;
        return 0x0001;
    }

    while(m_running)
    {
        onTick(m_tickCounter++);
    }

    result = onFinish(m_exitCode);
    if(result == EGE::EventResult::Failure)
    {
        std::cerr << "0002 EGE/loop: finish failed" << std::endl;
        return 0x0002;
    }

    return m_exitCode;
}

void MyGameLoop::exit(int exitCode)
{
    m_exitCode = exitCode;
    m_running = false;
    onExit(exitCode);
}

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
    m_window = make<EGE::SFMLSystemWindow>();
    addEventHandler(EGE::SystemEvent::getTypeStatic(), new MySystemEventHandler(m_window));
    m_window->create(sf::VideoMode(256, 256), "EGE Test");
    return EGE::EventResult::Success;
}

MyGameLoop loop;

TESTCASE(createWindow)
{
    return loop.run();
}

RUN_TESTS(syswindow)
