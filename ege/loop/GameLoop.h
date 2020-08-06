/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#pragma once

#include "EventHandler.h"
#include "Timer.h"

#include <map>
#include <memory>

namespace EGE
{

class GameLoop
{
public:
    EGE_ENUM_YES_NO(TimerImmediateStart);

    int run();
    void exit(int exitCode = 0);
    bool isRunning() { return m_running; }
    long long getTickCount() { return m_tickCounter; }

    template<class T>
    void addEventHandler(Event::EventType type, T* handler)
    {
        ASSERT(handler);
        return addEventHandler(type, std::shared_ptr<T>(handler));
    }

    void addEventHandler(Event::EventType type, std::shared_ptr<EventHandler> handler);
    size_t getEventHandlerCount() { return m_eventHandlers.size(); }
    EventResult fireEvent(Event& event);

    virtual EventResult onLoad() = 0;
    virtual void onTick(long long tickCount) = 0;
    virtual void onExit(int exitCode) = 0;
    virtual EventResult onFinish(int exitCode) = 0;
    virtual void onTimerFinish(Timer* timer) {}
    virtual void onTimerTick(Timer* timer) {}

    void addTimer(const std::string& name, Timer* timer, TimerImmediateStart);
    Timer* getTimer(const std::string& timer);
    void removeTimer(const std::string& timer);
    virtual void updateTimers();

    // get in-loop time in ticks or ms
    virtual double time(Time::Unit unit);

private:
    int m_ticks = 0;
    std::map<std::string, std::shared_ptr<Timer>> m_timers;

private:
    bool m_running { true };
    int m_exitCode { 0 };
    long long m_tickCounter { 0 };
    std::multimap<Event::EventType, std::shared_ptr<EventHandler>> m_eventHandlers;
};

}
