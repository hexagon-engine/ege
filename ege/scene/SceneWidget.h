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

#include <SFML/Graphics.hpp>
#include <ege/gfx/RenderStates.h>
#include <ege/gui/Widget.h>
#include <ege/scene/Scene.h>

namespace EGE
{

class SceneWidget : public Widget
{
public:
    explicit SceneWidget(Widget& parent, SharedPtr<Scene> initialScene = nullptr)
    : Widget(parent, "SceneWidget"), m_initialScene(initialScene) {}

    virtual void render(Renderer& renderer) const override;
    virtual void onUpdate(long long tickCounter) override;

    void setScene(SharedPtr<Scene> scene)
    {
        ASSERT(scene);
        m_scene = scene;
        runLayoutUpdate();
        m_scene->setSize(getSize());
    }

    SharedPtr<Scene> getScene() { return m_scene; }

    void setCamera(SharedPtr<Camera> cameraObject) { m_cameraObject = cameraObject; }

    Vec2d mapToScreenCoords(Renderer& renderer, Vec3d scene) const;
    Vec3d mapToSceneCoords(Renderer& renderer, Vec2d screen) const;

protected:
    virtual void updateGeometry(Renderer&) override;

private:
    SharedPtr<Scene> m_scene;
    SharedPtr<Scene> m_initialScene;
    WeakPtr<Camera> m_cameraObject;
};

}
