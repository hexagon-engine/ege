/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#include "GameLoop.h"

#include "EventResult.h"
#include "TimerEvent.h"

#include <iostream>
#include <main/Config.h>
#include <string.h>

namespace EGE
{

int GameLoop::run()
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

void GameLoop::exit(int exitCode)
{
    m_exitCode = exitCode;
    m_running = false;
    onExit(exitCode);
}

void GameLoop::addEventHandler(Event::EventType type, std::shared_ptr<EventHandler> handler)
{
    m_eventHandlers.insert(std::make_pair(type, handler));
}

EventResult GameLoop::fireEvent(Event& event)
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

void GameLoop::addTimer(const std::string& name, Timer* timer, GameLoop::TimerImmediateStart immediateStart)
{
    ASSERT(timer);
    timer->setName(name);
    if(immediateStart == GameLoop::TimerImmediateStart::Yes)
    {
        TimerEvent event(TimerEvent::Start, timer);
        fireEvent(event);
        if(event.isCanceled())
        {
            delete timer;
            return;
        }
        timer->start();
    }
    if(!timer->getCallback())
    {
        timer->setCallback([this](std::string name, Timer* timer) {
                                TimerEvent event(TimerEvent::Tick, timer);
                                fireEvent(event);
                                if(event.isCanceled())
                                    return;
                                this->onTimerTick(timer);
                           });
    }
    m_timers.insert(std::make_pair(name, std::shared_ptr<Timer>(timer)));
}

Timer* GameLoop::getTimer(const std::string& timer)
{
    auto it = m_timers.find(timer);
    if(it != m_timers.end())
        return it->second.get();
    return nullptr;
}
void GameLoop::removeTimer(const std::string& timer)
{
    auto it = m_timers.find(timer);
    if(it != m_timers.end())
        m_timers.erase(it);
}
void GameLoop::updateTimers()
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
            it = m_timers.find(timer.first);
        }
    }
    m_ticks++;
}
double GameLoop::time(Time::Unit unit)
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
