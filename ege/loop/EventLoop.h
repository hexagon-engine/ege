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

#pragma once

#include "EventHandler.h"

#include "Timer.h"

#include <map>
#include <memory>
#include <queue>
#include <vector>

namespace EGE
{

class EventLoop
{
public:
    EGE_ENUM_YES_NO(TimerImmediateStart);

    void addEventHandler(Event::EventType type, EventHandler* handler)
    {
        ASSERT(handler);
        addEventHandler(type, std::shared_ptr<EventHandler>(handler));
    }

    virtual void addEventHandler(Event::EventType type, std::shared_ptr<EventHandler> handler);

    // NOTE: removing event handlers in event handler is UB
    // TODO: it's slow
    virtual void removeEventHandler(EventHandler* handler);

    virtual size_t getEventHandlerCount() { return m_eventHandlers.size(); }
    virtual EventResult fireEvent(Event& event);

    virtual void onTimerFinish(Timer*) {}
    virtual void onTimerTick(Timer*) {}

    void addTimer(const std::string& name, Timer* timer, TimerImmediateStart start = TimerImmediateStart::Yes)
    {
        ASSERT(timer);
        addTimer(name, std::shared_ptr<Timer>(timer), start);
    }

    virtual void addTimer(const std::string& name, std::shared_ptr<Timer> timer, TimerImmediateStart start = TimerImmediateStart::Yes);
    virtual std::vector<std::weak_ptr<Timer>> getTimers(const std::string& timer);
    virtual void removeTimer(const std::string& timer);
    virtual void onUpdate();
    virtual void deferredInvoke(std::function<void()> func);

    // get in-loop time in ticks or ms
    // it should be used ONLY for comparisions
    virtual double time(Time::Unit unit);

    virtual int run();

    virtual void exit(int exitCode = 0);
    bool isRunning() { return m_running; }
    long long getTickCount() { return m_ticks; }

    virtual bool setSubLoop(std::shared_ptr<EventLoop> loop)
    {
        m_subLoop = loop;
        return true;
    }
    std::shared_ptr<EventLoop> getSubLoop()
    {
        return m_subLoop;
    }

protected:
    virtual void updateTimers();
    virtual void callDeferredInvokes();
    bool m_running = true;
    int m_exitCode = 0;

private:
    int m_ticks = 0;
    std::multimap<std::string, std::shared_ptr<Timer>> m_timers;
    std::multimap<Event::EventType, std::shared_ptr<EventHandler>> m_eventHandlers;
    std::shared_ptr<EventLoop> m_subLoop;
    std::queue<std::function<void()>> m_deferredInvokes;
};

}
