/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#include "EGEGame.h"

namespace EGE
{

bool EGEGame::GPOM::clear()
{
    sceneObjectCreators.clear();
    return true;
}

bool EGEGame::GPOM::load()
{
    return m_game->registerSceneObjectCreators(this);
}

bool EGEGame::initialize()
{
    m_gameplayObjectManager = std::make_shared<EGEGame::GPOM>(this);
    return m_gameplayObjectManager->reload();
}

}
