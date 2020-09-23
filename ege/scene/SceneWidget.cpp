/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#include "SceneWidget.h"

#include <algorithm>
#include <ege/gui/GUIGameLoop.h>

namespace EGE
{

void SceneWidget::renderOnly(sf::RenderTarget& target)
{
    Widget::renderOnly(target);
    if(m_scene)
        m_scene->renderOnly(target);
}

void SceneWidget::onResize(sf::Event::SizeEvent& event)
{
    if(m_autoResizable)
    {
        m_size = EGE::Vec2d(event.width, event.height);

        if(m_scene)
            m_scene->setSize(sf::Vector2f(m_size.x, m_size.y));
    }
}

void SceneWidget::onUpdate(long long tickCounter)
{
    Widget::onUpdate(tickCounter);
    if(m_scene)
    {
        if(m_gameLoop->getProfiler()) m_gameLoop->getProfiler()->startSection("sceneUpdate");
        m_scene->onUpdate(tickCounter);
        if(m_gameLoop->getProfiler()) m_gameLoop->getProfiler()->endSection();
    }
}

void SceneWidget::setViewForWidget(sf::RenderTarget& target)
{
    // fill the window fully by default
    if(m_size == EGE::Vec2d())
    {
        auto size = target.getSize();
        m_size = EGE::Vec2d(size.x, size.y);
        if(m_scene)
            m_scene->setSize(sf::Vector2f(m_size.x, m_size.y));

        m_autoResizable = true;
    }

    Widget::setViewForWidget(target);
}

}
