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
    if(result == EGE::EventResult::Failure)
    {
        std::cerr << "0001 EGE/loop: load failed" << std::endl;
        return 0x0001;
    }

    while(m_running)
    {
        onTick(getTickCount());
        onUpdate();
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
    EventLoop::exit(exitCode);
    onExit(exitCode);
}

}
