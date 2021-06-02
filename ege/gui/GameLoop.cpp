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

#include "GameLoop.h"

#include <ege/debug/Inspector.h>
#include <ege/debug/Logger.h>
#include <ege/core/Clock.h>
#include <ege/core/EventResult.h>
#include <SFML/System.hpp>

namespace EGE
{

int GameLoop::run()
{
    auto result = onLoad();

    if(result == EventResult::Failure)
    {
        ege_log.critical() << "GameLoop: onLoad() failed";
        return 0x0001;
    }

    // TODO: maybe our own clocks?
    Clock tickClock(this);
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
            sf::sleep(sf::seconds(m_minTickTime.getValue() - tickClock.getElapsedTime()));

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
        ege_log.critical() << "GameLoop: onFinish() failed";
        return 0x0002;
    }

    EGE::Inspector::instance().display(EGE::mainLogger());
    return m_exitCode;
}

bool GameLoop::setSubLoop(SharedPtr<GameLoop> loop)
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
