/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#include "EGEGame.h"

#include <ege/debug/Logger.h>
#include <ege/scene/CameraObject2D.h>
#include <functional>

namespace EGE
{

bool EGEGame::GPOM::clear()
{
    sceneObjectCreators.clear();
    return true;
}

bool EGEGame::GPOM::load()
{
    sceneObjectCreators.add("EGE::CameraObject2D", new std::function([](std::shared_ptr<EGE::Scene> scene)->std::shared_ptr<SceneObject> {
                                                                        return make<CameraObject2D>(scene);
                                                                     }));
    return true;
}

EGEGame::EGEGame()
{
    m_gameplayObjectManager = make<EGEGame::GPOM>(this);
}

void EGEGame::setScene(std::shared_ptr<Scene> scene)
{
    m_scene = scene;
}

bool EGEGame::initialize()
{
    if(m_versionString.empty())
    {
        log(LogLevel::Warning) << "EGEGame: Version string is empty!";
        m_versionString = "Game";
    }

    return m_gameplayObjectManager->load();
}

}
