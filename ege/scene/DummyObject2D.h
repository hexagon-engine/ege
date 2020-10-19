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

    void setSize(sf::Vector2f size)
    {
        m_size = size;
    }
    sf::Vector2f getSize() const
    {
        return m_size;
    }
    sf::FloatRect getBoundingBox()
    {
        return sf::FloatRect(getPosition(), m_size);
    }
    virtual void render(sf::RenderTarget& target, const RenderStates& states) const;

private:
    sf::Vector2f m_size;
};

}
