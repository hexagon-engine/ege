/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#pragma once

#include "EventHandler.h"

#include <map>
#include <memory>

namespace EGE
{

class GameLoop
{
public:
    int run();
    void exit(int exitCode = 0);
    bool isRunning() { return m_running; }
    long long getTickCount() { return m_tickCounter; }
    void addEventHandler(Event::EventType type, EventHandler* handler);
    size_t getEventHandlerCount() { return m_eventHandlers.size(); }
    EventResult fireEvent(Event::EventType type, Event& event);
    void addTimer(std::string name, Timer* timer);
    void removeTimer(std::string name);

    virtual EventResult onLoad() = 0;
    virtual void onTick(long long tickCount) = 0;
    virtual void onExit(int exitCode) = 0;
    virtual EventResult onFinish(int exitCode) = 0;
    virtual void onTimerFinish(std::string timerName) {}
    virtual void onTimerTick(std::string timerName) {}

private:
    void updateTimers();

    bool m_running { true };
    int m_exitCode { 0 };
    long long m_tickCounter { 0 };
    std::multimap<Event::EventType, std::shared_ptr<EventHandler>> m_eventHandlers;
    std::map<std::string, std::shared_ptr<Timer>> m_timers;
};

}
