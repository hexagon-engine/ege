/*
*
*   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*
*      ,----  ,----  ,----
*      |      |      |
*      |----  | --,  |----
*      |      |   |  |
*      '----  '---'  '----
*
*     Framework Library for Hexagon
*
*    Copyright (c) Sppmacd 2020 - 2021
*
*    Permission is hereby granted, free of charge, to any person obtaining a copy
*    of this software and associated documentation files (the "Software"), to deal
*    in the Software without restriction, including without limitation the rights
*    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
*    copies of the Software, and to permit persons to whom the Software is
*    furnished to do so, subject to the following conditions:
*
*    The above copyright notice and this permission notice shall be included in all
*    copies or substantial portions of the Software.
*
*    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
*    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
*    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
*    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
*    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
*    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
*    SOFTWARE.
*
*   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*
*/

#include <testsuite/Tests.h>
#include <ege/core/MainLoop.h>
#include <ege/debug/Logger.h>
#include <ege/event/SystemEvent.h>
#include <ege/event/DefaultSystemEventHandler.h>
#include <ege/event/SystemWindow.h>

class MyGameLoop : public EGE::MainLoop
{
    EGE::SharedPtr<EGE::SFMLSystemWindow> m_window;

public:
    virtual EGE::EventResult onLoad() override;

    virtual void onTick() override
    {
        //DEBUG_PRINT("onTick");
        m_window->callEvents(*this);
        if(!m_window->isOpen())
            exit();
        m_window->clear();
        m_window->display();
    }
};

class MySystemEventHandler : public EGE::DefaultSystemEventHandler
{
public:
    MySystemEventHandler(EGE::SFMLSystemWindow& window)
    : EGE::DefaultSystemEventHandler(window)
    {}

    virtual void onMouseEnter()
    {
        ege_log.info() << "onMouseEnter";
    }
    virtual void onMouseLeave()
    {
        ege_log.info() << "onMouseLeave";
    }
};

EGE::EventResult MyGameLoop::onLoad()
{
    m_window = make<EGE::SFMLSystemWindow>();
    events<EGE::SystemEvent>().addHandler<MySystemEventHandler>(*m_window);
    m_window->create(sf::VideoMode(256, 256), "EGE Test");
    return EGE::EventResult::Success;
}

MyGameLoop loop;

TESTCASE(createWindow)
{
    return loop.run();
}

RUN_TESTS(syswindow)
