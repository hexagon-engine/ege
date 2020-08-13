/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#include "Game.h"

namespace EGE
{

Game::Game()
{
    m_gameLoop = std::make_shared<GUIGameLoop>();
    m_profiler = std::make_shared<Profiler>();
}

int Game::run()
{
    if(m_gameplayObjectManager)
        m_gameplayObjectManager->reload();
    m_gameLoop->setProfiler(getProfiler());
    return m_gameLoop->run();
}

}
