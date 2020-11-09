/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#pragma once

#include <ege/asyncLoop/ThreadSafeEventLoop.h>
#include <ege/debug/Profiler.h>
#include <ege/loop/EventHandler.h>
#include <ege/loop/EventResult.h>
#include <ege/loop/Timer.h>
#include <ege/util/Time.h>

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

    // It also calls onLoad for loop. Beware of it when you are setting
    // EGE::EGEClient or EGE::EGEServer, which will be started now
    // (and it creates threads, opens ports etc.)
    virtual bool setSubLoop(std::shared_ptr<GameLoop> loop);

    virtual void setMinimalTickTime(Time time)
    {
        m_minTickTime = time;
    }

    std::shared_ptr<Profiler> getProfiler()
    {
        return m_profiler;
    }

protected:
    std::shared_ptr<Profiler> m_profiler;

private:
    virtual GameLoop* getSubGameLoop();

    Time m_minTickTime = {0.0, Time::Unit::Seconds};
};

}
