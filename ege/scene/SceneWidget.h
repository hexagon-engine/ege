/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
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
    SceneWidget(Widget* parent, std::shared_ptr<Scene> initialScene = nullptr)
    : Widget(parent), m_scene(initialScene) {}

    virtual void renderOnly(sf::RenderTarget& target, const RenderStates& states);

    virtual void onUpdate(long long tickCounter);
    virtual void onResize(sf::Event::SizeEvent& event);

    void setSize(EGE::Vec2d size)
    {
        m_size = size;
        if(m_scene)
            m_scene->setSize(sf::Vector2f(size.x, size.y));

        m_autoResizable = false;
    }
    EGE::Vec2d getSize()
    {
        return m_size;
    }

    void setScene(std::shared_ptr<Scene> scene)
    {
        ASSERT(scene);
        m_scene = scene;
        m_scene->setSize(sf::Vector2f(m_size.x, m_size.y));
    }

    std::shared_ptr<Scene> getScene()
    {
        return m_scene;
    }

    // The Scene widget is set to be auto resizable if its size is not set
    // before first render. You can revert this behaviour by using setSize().
    virtual void setViewForWidget(sf::RenderTarget& target);

private:
    bool m_autoResizable = false;
    std::shared_ptr<Scene> m_scene;
};

}
