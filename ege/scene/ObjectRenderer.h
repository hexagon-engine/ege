/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#pragma once

#include <memory>
#include <SFML/Graphics.hpp>

#include <ege/gfx/RenderStates.h>

namespace EGE
{

class Scene;
class SceneObject;

class ObjectRenderer
{
public:
    ObjectRenderer(std::shared_ptr<Scene> scene)
    : m_scene(scene) {}

    virtual void render(const SceneObject& object, sf::RenderTarget& target, const RenderStates& states) const = 0;

    virtual void preRender(SceneObject& object, sf::RenderTarget&)
    {
        if(m_geometryNeedUpdate)
        {
            updateGeometry(object);
            m_geometryNeedUpdate = false;
        }
    }

    virtual void updateGeometry(SceneObject&) {}

    void setGeometryNeedUpdate()
    {
        m_geometryNeedUpdate = true;
    }

protected:
    std::shared_ptr<Scene> m_scene;

private:
    bool m_geometryNeedUpdate = true;
};

}
