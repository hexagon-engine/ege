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
#include <ege/egeNetwork/ControlPacket.h>
#include <ege/gpo/GameplayObjectManager.h>
#include <ege/gpo/GameplayObjectRegistry.h>
#include <ege/scene/Scene.h>
#include <ege/scene/SceneLoader.h>

namespace EGE
{

class EGEGame
{
public:
    virtual ~EGEGame() = default;
    virtual void setScene(SharedPtr<Scene> scene);

    SharedPtr<Scene> getScene() { return m_scene; }

    // Connection is accepted if <peer version == version>
    void setVersion(int value) { m_version = value; }
    int getVersion() { return m_version; }

    // Set it to name of your game.
    void setVersionString(String value) { m_versionString = value; }
    String getVersionString() { return m_versionString; }

    using Controller = std::function<void(SceneObject&, SharedPtr<ObjectMap>)>;

    void registerController(String name, Controller controller);

protected:
    void runController(SceneObject& object, const ControlPacket& packet);

private:
    SharedPtr<Scene> m_scene;
    int m_version = 0; // 0 - "unknown"
    String m_versionString = "EGE Generic";
    Map<String, Controller> m_controllers;
};

}
