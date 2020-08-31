/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#pragma once

#include "EventHandler.h"

#include "Timer.h"

#include <map>
#include <memory>
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

    void addEventHandler(Event::EventType type, std::shared_ptr<EventHandler> handler);

    // NOTE: removing event handlers in event handler is UB
    // TODO: it's slow
    void removeEventHandler(EventHandler* handler);

    size_t getEventHandlerCount() { return m_eventHandlers.size(); }
    EventResult fireEvent(Event& event);

    virtual void onTimerFinish(Timer* timer) {}
    virtual void onTimerTick(Timer* timer) {}

    void addTimer(const std::string& name, Timer* timer, TimerImmediateStart start = TimerImmediateStart::Yes)
    {
        ASSERT(timer);
        addTimer(name, std::shared_ptr<Timer>(timer), start);
    }

    void addTimer(const std::string& name, std::shared_ptr<Timer> timer, TimerImmediateStart start = TimerImmediateStart::Yes);
    std::vector<std::weak_ptr<Timer>> getTimers(const std::string& timer);
    void removeTimer(const std::string& timer);
    virtual void onUpdate();
    void deferredInvoke(std::function<void()> func);

    // get in-loop time in ticks or ms
    virtual double time(Time::Unit unit);

protected:
    virtual void updateTimers();

private:
    int m_ticks = 0;
    std::multimap<std::string, std::shared_ptr<Timer>> m_timers;
    std::multimap<Event::EventType, std::shared_ptr<EventHandler>> m_eventHandlers;
};

}
