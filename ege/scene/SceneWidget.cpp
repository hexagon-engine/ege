/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#include "SceneWidget.h"

#include <algorithm>
#include <ege/gui/GUIGameLoop.h>

namespace EGE
{

void SceneWidget::render(Renderer& renderer) const
{
    if(m_scene)
        m_scene->doRender(renderer);
}

void SceneWidget::onResize(sf::Event::SizeEvent& event)
{
    if(m_autoResizable)
    {
        m_size = EGE::Vec2d(event.width, event.height);

        if(m_scene)
            m_scene->setSize(m_size);
    }
}

void SceneWidget::onUpdate(long long tickCounter)
{
    Widget::onUpdate(tickCounter);
    if(m_scene)
    {
        if(m_gameLoop.getProfiler()) m_gameLoop.getProfiler()->startSection("sceneUpdate");
        m_scene->onUpdate(tickCounter);
        if(m_gameLoop.getProfiler()) m_gameLoop.getProfiler()->endSection();
    }
}

void SceneWidget::updateGeometry(Renderer& renderer)
{
    // The first geometry update (always done before first render)
    // should set the widget auto resizable if it's not explicitly
    // disabled by user.
    if(m_size == EGE::Vec2d())
    {
        auto size = renderer.getTarget().getSize();
        m_size = EGE::Vec2d(size.x, size.y);
        if(m_scene)
            m_scene->setSize(m_size);

        m_autoResizable = true;
    }
}

}
