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
