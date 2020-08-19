/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#pragma once

#include "SceneObject.h"

#include <ege/gui/AnimationEasingFunctions.h>
#include <functional>
#include <SFML/Graphics.hpp>

namespace EGE
{

class SceneObject2D : public SceneObject
{

public:
    SceneObject2D(EGE::Scene* owner, std::string name)
    : SceneObject(owner, name) {}

    void setPosition(sf::Vector2f position)
    {
        m_position = position;
    }
    sf::Vector2f getPosition() const
    {
        return m_position;
    }

    virtual sf::FloatRect getBoundingBox()
    {
        return sf::FloatRect(m_position, sf::Vector2f(0.f, 0.f));
    }

    // with collision check
    virtual bool moveTo(sf::Vector2f pos);

    // with collision check
    virtual bool flyTo(sf::Vector2f pos, float time, std::function<double(double)> easing = AnimationEasingFunctions::linear);

    virtual void render(sf::RenderTarget& target) const = 0;

private:
    sf::Vector2f m_position;
};

}
