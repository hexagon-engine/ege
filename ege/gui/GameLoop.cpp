/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#include "GameLoop.h"

#include <ege/debug/Logger.h>
#include <ege/loop/EventResult.h>
#include <SFML/System.hpp>

namespace EGE
{

int GameLoop::run()
{
    auto result = onLoad();

    if(result == EventResult::Failure)
    {
        err(LogLevel::Critical) << "0001 EGE/gui: load failed";
        return 0x0001;
    }

    // TODO: maybe our own clocks?
    sf::Clock tickClock;
    while(m_running)
    {
        tickClock.restart();
        onTick(getTickCount());

        auto subLoop = getSubGameLoop();
        if(subLoop)
            subLoop->onTick(getTickCount());

        onUpdate();

        // Limit tick time / frame rate
        if(m_minTickTime.getValue() > 0.0)
            sf::sleep(sf::seconds(m_minTickTime.getValue()) - tickClock.getElapsedTime());
    }

    result = onFinish(m_exitCode);

    auto subLoop = getSubGameLoop();
    if(subLoop)
    {
        auto result2 = subLoop->onFinish(m_exitCode);
        if(result2 == EventResult::Failure)
            result = EventResult::Failure;
    }

    if(result == EventResult::Failure)
    {
        err(LogLevel::Critical) << "0002 EGE/gui: finish failed";
        return 0x0002;
    }

    return m_exitCode;
}

void GameLoop::setSubLoop(std::shared_ptr<GameLoop> loop)
{
    ASSERT(loop);
    loop->onLoad();
    EventLoop::setSubLoop(loop);
}

void GameLoop::exit(int exitCode)
{
    EventLoop::exit(exitCode);
    onExit(exitCode);

    auto subLoop = getSubGameLoop();
    if(subLoop)
    {
        subLoop->onExit(m_exitCode);
    }
}

GameLoop* GameLoop::getSubGameLoop()
{
    return std::dynamic_pointer_cast<GameLoop>(getSubLoop()).get();
}

}
