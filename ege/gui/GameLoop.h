/*
*
*   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*
*      ,----  ,----  ,----
*      |      |      |
*      |----  | --,  |----
*      |      |   |  |
*      '----  '---'  '----
*
*     Framework Library for Hexagon
*
*    Copyright (c) Sppmacd 2020 - 2021
*
*    Permission is hereby granted, free of charge, to any person obtaining a copy
*    of this software and associated documentation files (the "Software"), to deal
*    in the Software without restriction, including without limitation the rights
*    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
*    copies of the Software, and to permit persons to whom the Software is
*    furnished to do so, subject to the following conditions:
*
*    The above copyright notice and this permission notice shall be included in all
*    copies or substantial portions of the Software.
*
*    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
*    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
*    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
*    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
*    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
*    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
*    SOFTWARE.
*
*   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*
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
