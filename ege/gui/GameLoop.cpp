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

    if(getSubLoop())
        getSubLoop()->onLoad(getTickCount());

    if(result == EventResult::Failure)
    {
        std::cerr << "0001 EGE/loop: load failed" << std::endl;
        return 0x0001;
    }

    while(m_running)
    {
        onTick(getTickCount());

        if(getSubLoop())
            getSubLoop()->onTick(getTickCount());

        onUpdate();
    }

    result = onFinish(m_exitCode);

    if(getSubLoop())
    {
        auto result2 = getSubLoop()->onFinish(m_exitCode);
        if(result2 == EventResult::Failure)
            return = EventResult::Failure;
    }

    if(result == EventResult::Failure)
    {
        std::cerr << "0002 EGE/loop: finish failed" << std::endl;
        return 0x0002;
    }

    return m_exitCode;
}

void GameLoop::setSubLoop(std::shared_ptr<EventLoop> loop)
{
    ASSERT(loop);
    loop->onLoad();
    EventLoop::setSubLoop(loop);
}

void GameLoop::exit(int exitCode)
{
    EventLoop::exit(exitCode);
    onExit(exitCode);
    if(getSubLoop())
        getSubLoop()->onExit(exitCode);
}

}
