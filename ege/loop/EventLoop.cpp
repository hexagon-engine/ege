/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
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

void EventLoop::addEventHandler(Event::EventType type, std::shared_ptr<EventHandler> handler)
{
    m_eventHandlers.insert(std::make_pair(type, handler));
}

void EventLoop::removeEventHandler(EventHandler* handler)
{
    for(auto it = m_eventHandlers.begin(); it != m_eventHandlers.end(); it++)
    {
        if(it->second.get() == handler)
        {
            m_eventHandlers.erase(it);
            return;
        }
    }
}

EventResult EventLoop::fireEvent(Event& event)
{
    EventResult result = EventResult::Success;
    Event::EventType type = event.getType();
    ASSERT(!type.empty());
    for(auto& pr: m_eventHandlers)
    {
        ASSERT(!pr.first.empty());
        //std::cerr << "GameLoop: " << pr.first << " == " << type << std::endl;
        if(pr.first == type)
            (bool&)result |= (bool)pr.second->handle(event);
    }
    return result;
}

void EventLoop::addTimer(const std::string& name, std::shared_ptr<Timer> timer, EventLoop::TimerImmediateStart immediateStart)
{
    ASSERT(timer);
    timer->setName(name);
    if(immediateStart == EventLoop::TimerImmediateStart::Yes)
    {
        TimerEvent event(TimerEvent::Start, timer.get());
        fireEvent(event);
        if(event.isCanceled())
        {
            return;
        }
        timer->start();
    }
    if(!timer->getCallback())
    {
        timer->setCallback([this](std::string, Timer* _timer) {
                                TimerEvent event(TimerEvent::Tick, _timer);
                                fireEvent(event);
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
            TimerEvent event(TimerEvent::Finish, timer.second.get());
            fireEvent(event);
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
