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

#include "SceneWidget.h"

#include <algorithm>
#include <ege/gui/GUIGameLoop.h>

namespace EGE
{

void SceneWidget::render(Renderer& renderer) const
{
    if(m_scene)
    {
        if(!m_cameraObject.expired())
        {
            auto camera = m_cameraObject.lock();
            camera->applyTransform(renderer);
            m_scene->m_currentCamera = camera.get();
        }

        m_scene->doRender(renderer, renderer.getStates());
        m_scene->m_currentCamera = nullptr;
    }
}

void SceneWidget::onUpdate(long long tickCounter)
{
    Widget::onUpdate(tickCounter);

    if(!m_scene && m_initialScene)
        setScene(m_initialScene);

    if(m_scene)
    {
        if(m_gameLoop.getProfiler()) m_gameLoop.getProfiler()->startSection("sceneUpdate");
        m_scene->onUpdate(tickCounter);
        if(m_gameLoop.getProfiler()) m_gameLoop.getProfiler()->endSection();
    }
}

void SceneWidget::updateGeometry(Renderer&)
{
    if(m_scene)
        m_scene->setSize(getSize());
}

Vec2d SceneWidget::mapToScreenCoords(Renderer& renderer, Vec3d scene) const
{
    return m_cameraObject.expired() ? m_cameraObject.lock()->mapToScreenCoords(renderer, scene) : scene.toVec2d();
}

Vec3d SceneWidget::mapToSceneCoords(Renderer& renderer, Vec2d screen) const
{
    return m_cameraObject.expired() ? m_cameraObject.lock()->mapToSceneCoords(renderer, screen) : screen;
}

}
