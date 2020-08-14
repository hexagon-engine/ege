/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#pragma once

#include "EventHandler.h"
#include "EventLoop.h"
#include "EventResult.h"
#include "Timer.h"

#include <map>
#include <memory>

namespace EGE
{

class GameLoop : public EventLoop
{
public:
    int run();
    void exit(int exitCode = 0);
    bool isRunning() { return m_running; }
    long long getTickCount() { return m_tickCounter; }

    // NOTE: it's synchronous load so cannot be used to display loading screen
    virtual EventResult onLoad() = 0;
    virtual void onTick(long long tickCount) = 0;
    virtual void onExit(int exitCode) = 0;
    virtual EventResult onFinish(int exitCode) = 0;

private:
    bool m_running = true;
    int m_exitCode = 0;
    long long m_tickCounter = 0;
};

}
