/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#pragma once

#include "SceneObject2D.h"
#include "Scene.h"

#include <ege/gfx/RenderStates.h>
#include <SFML/Graphics.hpp>

namespace EGE
{

// invisible wall :)
class DummyObject2D : public SceneObject2D
{
public:
    DummyObject2D(std::shared_ptr<EGE::Scene> owner, std::string name)
    : SceneObject2D(owner, name) {}

    void setSize(Vec2d size)
    {
        m_size = size;
    }
    Vec2d getSize() const
    {
        return m_size;
    }
    sf::FloatRect getBoundingBox()
    {
        return sf::FloatRect(getPosition().x, getPosition().y, m_size.x, m_size.y);
    }

    virtual void render(Renderer&) const override {}

private:
    Vec2d m_size;
};

}
