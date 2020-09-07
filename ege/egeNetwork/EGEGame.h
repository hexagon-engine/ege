/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#pragma once

#include <ege/gpo/GameplayObjectManager.h>
#include <ege/gpo/GameplayObjectRegistry.h>
#include <ege/scene/Scene.h>

namespace EGE
{

class EGEGame
{
public:
    // gameplay object manager
    class GPOM : public GameplayObjectManager
    {
    public:
        GPOM(EGEGame* game)
        : m_game(game) {}

        bool clear();
        bool load();

        GameplayObjectRegistry<std::string, std::function<std::shared_ptr<SceneObject>()>> sceneObjectCreators;

    private:
        EGEGame* m_game;
    };

    void setScene(std::shared_ptr<Scene> scene)
    {
        m_scene = scene;
    }

    std::shared_ptr<Scene> getScene()
    {
        return m_scene;
    }

    std::shared_ptr<GPOM> getGameplayObjectManager()
    {
        return m_gameplayObjectManager;
    }

    virtual bool initialize();

    // to be overridden by user
    virtual bool registerSceneObjectCreators(GPOM*) { return true; }

private:
    std::shared_ptr<Scene> m_scene;
    std::shared_ptr<GPOM> m_gameplayObjectManager;
};

}
