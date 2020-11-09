/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
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

    std::shared_ptr<GUIGameLoop> getLoop()
    {
        return m_gameLoop;
    }
    void setLoop(std::shared_ptr<GUIGameLoop> loop)
    {
        ASSERT(!m_gameLoop);
        m_gameLoop = loop;
    }

    std::shared_ptr<GameplayObjectManager> getGameplayObjectManager()
    {
        ASSERT(m_gameplayObjectManager);
        return m_gameplayObjectManager;
    }
    void setGameplayObjectManager(std::shared_ptr<GameplayObjectManager> gpom)
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
    std::shared_ptr<Profiler> m_profiler;
    std::shared_ptr<GameplayObjectManager> m_gameplayObjectManager;
    std::shared_ptr<GUIGameLoop> m_gameLoop;
};

}
