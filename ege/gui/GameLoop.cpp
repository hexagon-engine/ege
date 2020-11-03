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
        m_profiler = make<Profiler>();
        m_profiler->start();
        m_profiler->startSection("tick");

        tickClock.restart();
        onTick(getTickCount());

        m_profiler->endStartSection("subLoopTick");
        auto subLoop = getSubGameLoop();
        if(subLoop)
            subLoop->onTick(getTickCount());

        m_profiler->endStartSection("update");
        onUpdate();

        // Limit tick time / frame rate
        m_profiler->endStartSection("tickLimit");
        if(m_minTickTime.getValue() > 0.0)
            sf::sleep(sf::seconds(m_minTickTime.getValue()) - tickClock.getElapsedTime());

        m_profiler->endSection();
        m_profiler->end();
        //std::cerr << "TPS: " << 1.f / tickClock.getElapsedTime().asSeconds() << std::endl;
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

bool GameLoop::setSubLoop(std::shared_ptr<GameLoop> loop)
{
    ASSERT(loop);
    if(loop->onLoad() == EventResult::Failure)
        return false;
    EventLoop::setSubLoop(loop);
    return true;
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
