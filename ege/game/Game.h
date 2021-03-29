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

#include <ege/debug/Profiler.h>
#include <ege/gpo/GameplayObjectManager.h>
#include <ege/gui/GUIGameLoop.h>
#include <memory>

namespace EGE
{

class Game
{
public:
    EGE_SINGLETON(Game);
    Game();

    SharedPtr<GUIGameLoop> getLoop()
    {
        return m_gameLoop;
    }
    void setLoop(SharedPtr<GUIGameLoop> loop)
    {
        ASSERT(!m_gameLoop);
        m_gameLoop = loop;
    }

    SharedPtr<GameplayObjectManager> getGameplayObjectManager()
    {
        ASSERT(m_gameplayObjectManager);
        return m_gameplayObjectManager;
    }
    void setGameplayObjectManager(SharedPtr<GameplayObjectManager> gpom)
    {
        ASSERT(gpom);
        m_gameplayObjectManager = gpom;
    }

    std::weak_ptr<Profiler> getProfiler()
    {
        return m_profiler;
    }

    int run();

protected:
    SharedPtr<Profiler> m_profiler;
    SharedPtr<GameplayObjectManager> m_gameplayObjectManager;
    SharedPtr<GUIGameLoop> m_gameLoop;
};

}
