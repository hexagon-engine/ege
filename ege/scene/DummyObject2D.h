#pragma once

#include "SceneObject2D.h"
/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#include "Scene.h"

#include <SFML/Graphics.hpp>

namespace EGE
{

// invisible wall :)
class DummyObject2D : public SceneObject2D
{
public:
    DummyObject2D(Scene* owner, std::string name)
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
    virtual void render(sf::RenderTarget& target);

private:
    sf::Vector2f m_size;
};

}
