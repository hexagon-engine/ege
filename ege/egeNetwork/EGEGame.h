/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#pragma once

#include <ege/debug/Logger.h>
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

        GameplayObjectRegistry<std::string, std::function<std::shared_ptr<SceneObject>(std::shared_ptr<EGE::Scene>)>> sceneObjectCreators;

    private:
        EGEGame* m_game;
    };

    virtual void setScene(std::shared_ptr<Scene> scene);

    std::shared_ptr<Scene> getScene() { return m_scene; }

    std::shared_ptr<GPOM> getGameplayObjectManager() { return m_gameplayObjectManager; }

    virtual bool initialize();

    virtual bool registerSceneObjectCreators(GPOM* gpom)
    {
        if(m_handler)
            return m_handler(gpom);
        else
        {
            log(LogLevel::Warning) << "EGEGame: GPOM handler not registered!";
            return true;
        }
    }

    void setInitializeHandler(std::function<bool(GPOM*)> handler) { m_handler = handler; }

    // Connection is accepted if <peer version == version>
    void setVersion(int value) { m_version = value; }
    int getVersion() { return m_version; }

    // Set it to name of your game.
    void setVersionString(std::string value) { m_versionString = value; }
    std::string getVersionString() { return m_versionString; }

private:
    std::shared_ptr<GPOM> m_gameplayObjectManager;
    std::shared_ptr<Scene> m_scene;
    std::function<bool(GPOM*)> m_handler;
    int m_version = 0; // 0 - "unknown"
    std::string m_versionString;
};

}
