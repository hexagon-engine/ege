/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#pragma once

#include <ege/asyncLoop/ThreadSafeEventLoop.h>
#include <ege/loop/EventHandler.h>
#include <ege/loop/EventResult.h>
#include <ege/loop/Timer.h>

#include <map>
#include <memory>

namespace EGE
{

class GameLoop : public ThreadSafeEventLoop
{
public:
    int run();
    virtual void exit(int exitCode = 0);

    // NOTE: it's synchronous load so cannot be used to display loading screen
    virtual EventResult onLoad() = 0;
    virtual void onTick(long long tickCount) = 0;
    virtual void onExit(int exitCode) = 0;
    virtual EventResult onFinish(int exitCode) = 0;
};

}
