/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#include "EventLoop.h"

#include "TimerEvent.h"

#include <ege/main/Config.h>
#include <iostream>
#include <string.h>

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
        timer->setCallback([this](std::string name, Timer* _timer) {
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
    updateTimers();
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
    addTimer("EGE::GameLoop::deferredInvoke() helper timer", &(new Timer(this, Timer::Mode::Limited, Time(0, Time::Unit::Ticks)))->setCallback(
        [func](std::string timerName, Timer* timer) {
            (void) timerName;
            (void) timer;
            func();
        }
    ));
}

double EventLoop::time(Time::Unit unit)
{
    if(unit == Time::Unit::Ticks)
        return m_ticks;
    else if(unit == Time::Unit::Seconds)
    {
        timespec _ts;
        if(clock_gettime(CLOCK_REALTIME, &_ts) < 0)
            return 0.0;

        // TODO: assume nobody will run our program longer than 1000000 seconds (~11 days)
        double time = (long long)(_ts.tv_sec) % 1000000 + _ts.tv_nsec / 1000000000.0;
        DUMP(TIMER_DEBUG, time);
        return time;
    }
    ASSERT(false);
    return 0.0;
}

}
