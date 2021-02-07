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

#include "EventLoop.h"

#include "TimerEvent.h"

#include <ege/main/Config.h>
#include <ege/util/PointerUtils.h>
#include <ege/util/system/Time.h>
#include <iostream>
#include <string.h>

#ifdef WIN32
    #include <windows.h>
#endif // WIN32

namespace EGE
{

EventLoop::EventArray<Event>& EventLoop::events(Event::EventType type)
{
    return m_eventHandlers[type];
}

void EventLoop::addTimer(const std::string& name, std::shared_ptr<Timer> timer, EventLoop::TimerImmediateStart immediateStart)
{
    ASSERT(timer);
    timer->setName(name);
    if(immediateStart == EventLoop::TimerImmediateStart::Yes)
    {
        TimerStartEvent event(*timer);
        events<TimerStartEvent>().fire(event);
        if(event.isCanceled())
        {
            return;
        }
        timer->start();
    }
    if(!timer->getCallback())
    {
        timer->setCallback([this](std::string, Timer* _timer) {
            TimerTickEvent event(*_timer);
            events<TimerTickEvent>().fire(event);

            if(event.isCanceled())
                return;
            this->onTimerTick(_timer);
       });
    }
    m_timers.insert(std::make_pair(name, timer));
}

std::vector<std::weak_ptr<Timer>> EventLoop::getTimers(const std::string& timer)
{
    std::vector<std::weak_ptr<Timer>> timers;
    decltype(m_timers)::iterator it;
    while((it = m_timers.find(timer)) != m_timers.end())
    {
        timers.push_back(it->second);
    }
    return timers;
}
void EventLoop::removeTimer(const std::string& timer)
{
    decltype(m_timers)::iterator it;
    while((it = m_timers.find(timer)) != m_timers.end())
    {
        m_timers.erase(it);
    }
}

void EventLoop::onUpdate()
{
    if(m_subLoop)
        m_subLoop->onUpdate();

    updateTimers();
    callDeferredInvokes();
    m_ticks++;
}

void EventLoop::updateTimers()
{
    for(auto it = m_timers.begin(); it != m_timers.end(); it++)
    {
        auto timer = *it;
        if(timer.second.get()->update() == Timer::Finished::Yes)
        {
            TimerFinishEvent event(*timer.second);
            events<TimerFinishEvent>().fire(event);
            if(event.isCanceled())
                continue;

            onTimerFinish(timer.second.get());
            m_timers.erase(it);

            if(m_timers.empty())
                return;

            it = m_timers.find(timer.first);
        }
    }
}

void EventLoop::deferredInvoke(std::function<void()> func)
{
    m_deferredInvokes.push(func);
}

void EventLoop::callDeferredInvokes()
{
    while(!m_deferredInvokes.empty())
    {
        m_deferredInvokes.front()();
        m_deferredInvokes.pop();
    }
}

double EventLoop::time(Time::Unit unit)
{
    if(unit == Time::Unit::Ticks)
        return m_ticks;
    else if(unit == Time::Unit::Seconds)
    {
        #if defined(WIN32)
            return GetTickCount() / 1000.0;
        #elif defined(__unix__)
            auto _time = System::exactTime();
            return _time.s + _time.ns / 1000000000.0;
        #endif
    }
    ASSERT(false);
}

void EventLoop::exit(int exitCode)
{
    m_exitCode = exitCode;
    m_running = false;
}

int EventLoop::run()
{
    while(m_running)
        onUpdate();
    return m_exitCode;
}

}
