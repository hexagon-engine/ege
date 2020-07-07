/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#include "GameLoop.h"

#include "EventResult.h"

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
    ASSERT(handler);
    m_eventHandlers.insert(std::make_pair(type, handler));
}

EventResult GameLoop::fireEvent(Event::EventType type, Event& event)
{
    EventResult result;
    for(auto& pr: m_eventHandlers)
    {
        ASSERT(!pr.first.empty());
        ASSERT(!type.empty());
        //std::cerr << "GameLoop: " << pr.first << " == " << type << std::endl;
        if(pr.first == type)
            (bool&)result |= (bool)pr.second->handle(event);
    }
    return result;
}

void GameLoop::updateTimers()
{
    for(auto& it = m_timers.begin(); it != m_timers.end(); it++)
    {
        Timer* timer = it->second;
        onTimerTick(timer->getName())
        timer->tick();
        if()
    }
}

}
