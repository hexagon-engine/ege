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
#include "ege/core/BasicComponent.h"

#include <algorithm>
#include <ege/gui/GUIGameLoop.h>

namespace EGE
{

void SceneWidget::render(Renderer& renderer) const
{
    auto scene = m_scene.lock();
    if(scene)
    {
        if(!m_cameraObject.expired())
        {
            auto camera = m_cameraObject.lock();
            camera->applyTransform(renderer);
            scene->m_currentCamera = camera.get();
        }

        scene->doRender(renderer, renderer.getStates());
        scene->m_currentCamera = nullptr;
    }
}

void SceneWidget::setScene(SharedPtr<Scene> scene)
{
    ASSERT(scene);
    m_scene = scene;
    runLayoutUpdate();
    scene->setSize(getSize());
}

void SceneWidget::onTick()
{
    Widget::onTick();

    auto scene = m_scene.lock();
    if(!scene && !m_initialScene.expired())
        setScene(m_initialScene.lock());

    // FIXME: This should be done by Component system somehow!
    if(scene)
        scene->onUpdate();
}

void SceneWidget::updateGeometry(Renderer&)
{
    if(!m_scene.expired())
        m_scene.lock()->setSize(getSize());
}

Vec2d SceneWidget::mapToScreenCoords(Renderer& renderer, Vec3d scene) const
{
    return m_cameraObject.expired() ? m_cameraObject.lock()->mapToScreenCoords(renderer, scene) : scene.toVec2d();
}

Vec3d SceneWidget::mapToSceneCoords(Renderer& renderer, Vec2d screen) const
{
    return m_cameraObject.expired() ? m_cameraObject.lock()->mapToSceneCoords(renderer, screen) : screen;
}

EventResult SceneWidget::fireEvent(Event& event)
{
    Widget::fireEvent(event);
    // FIXME: This should be done by Component system somehow!
    if(!m_scene.expired())
        m_scene.lock()->fireEvent(event);

    // TODO: Care about errors
    return EventResult::Success;
}

}
