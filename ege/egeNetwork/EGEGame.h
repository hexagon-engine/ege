/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#pragma once

#include <ege/debug/Logger.h>
#include <ege/gpo/GameplayObjectManager.h>
#include <ege/gpo/GameplayObjectRegistry.h>
#include <ege/scene/Scene.h>
#include <ege/scene/SceneLoader.h>

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

        SceneLoader::SceneObjectCreatorRegistry sceneObjectCreators;

    private:
        EGEGame* m_game;
    };

    EGEGame();

    virtual void setScene(std::shared_ptr<Scene> scene);

    std::shared_ptr<Scene> getScene() { return m_scene; }

    GPOM& getGameplayObjectManager() { return m_gameplayObjectManager; }

    void registerSceneObjectCreator(String typeId, SceneLoader::SceneObjectCreator creator)
    {
        m_gameplayObjectManager.sceneObjectCreators.addEntry(typeId, creator);
    }

    virtual bool initialize();

    // Connection is accepted if <peer version == version>
    void setVersion(int value) { m_version = value; }
    int getVersion() { return m_version; }

    // Set it to name of your game.
    void setVersionString(std::string value) { m_versionString = value; }
    std::string getVersionString() { return m_versionString; }

private:
    GPOM m_gameplayObjectManager;
    std::shared_ptr<Scene> m_scene;
    int m_version = 0; // 0 - "unknown"
    std::string m_versionString = "EGE Generic";
};

}
