/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#include "EGEGame.h"

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

    return m_game->registerSceneObjectCreators(this);
}

void EGEGame::setScene(std::shared_ptr<Scene> scene)
{
    m_scene = scene;
}

bool EGEGame::initialize()
{
    m_gameplayObjectManager = make<EGEGame::GPOM>(this);
    return m_gameplayObjectManager->reload();
}

}
