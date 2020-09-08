/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#include "GameLoop.h"

#include <ege/loop/EventResult.h>

namespace EGE
{

int GameLoop::run()
{
    auto result = onLoad();

    auto subLoop = getSubGameLoop();
    if(subLoop)
        subLoop->onLoad();

    if(result == EventResult::Failure)
    {
        std::cerr << "0001 EGE/loop: load failed" << std::endl;
        return 0x0001;
    }

    while(m_running)
    {
        onTick(getTickCount());

        auto subLoop = getSubGameLoop();
        if(subLoop)
            subLoop->onTick(getTickCount());

        onUpdate();
    }

    result = onFinish(m_exitCode);

    subLoop = getSubGameLoop();
    if(subLoop)
    {
        auto result2 = subLoop->onFinish(m_exitCode);
        if(result2 == EventResult::Failure)
            result = EventResult::Failure;
    }

    if(result == EventResult::Failure)
    {
        std::cerr << "0002 EGE/loop: finish failed" << std::endl;
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
