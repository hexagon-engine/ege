/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#pragma once

#include <SFML/Graphics.hpp>
#include <ege/gui/Widget.h>
#include <ege/scene/Scene.h>

namespace EGE
{

class SceneWidget : public Widget
{
public:
    SceneWidget(Widget* parent)
    : Widget(parent) {}

    virtual void renderOnly(sf::RenderTarget& target);

    virtual void onUpdate(long long tickCounter);
    virtual void onResize(sf::Event::SizeEvent& event);

    void setSize(sf::Vector2f size)
    {
        m_size = size;
        if(m_scene)
            m_scene->setSize(size);

        m_autoResizable = false;
    }
    sf::Vector2f getSize()
    {
        return m_size;
    }

    void setScene(std::shared_ptr<Scene> scene)
    {
        ASSERT(scene);
        m_scene = scene;
        m_scene->setSize(m_size);
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
