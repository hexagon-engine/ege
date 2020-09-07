/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#pragma once

#include "SceneObject.h"

#include <ege/gui/AnimationEasingFunctions.h>
#include <ege/main/Config.h>
#include <functional>
#include <SFML/Graphics.hpp>

#define SCENEOBJECT2D_DEBUG 1

namespace EGE
{

class SceneObject2D : public SceneObject
{

public:
    SceneObject2D(EGE::Scene* owner, std::string name)
    : SceneObject(owner, name) {}

    virtual void setPosition(sf::Vector2f position)
    {
        m_position = position;
    }
    sf::Vector2f getPosition() const
    {
        return m_position;
    }
    virtual void setMotion(sf::Vector2f motion)
    {
        m_motion = motion;
    }
    sf::Vector2f getMotion() const
    {
        return m_motion;
    }

    virtual sf::FloatRect getBoundingBox() const
    {
        return sf::FloatRect(m_position, sf::Vector2f(0.f, 0.f));
    }

    // with collision check
    virtual bool moveTo(sf::Vector2f pos);

    // with collision check
    virtual bool flyTo(sf::Vector2f pos, double time, std::function<double(double)> easing = AnimationEasingFunctions::linear);

    virtual void render(sf::RenderTarget& target) const;
    virtual void onUpdate(long long tickCounter);

    virtual std::shared_ptr<ObjectMap> serializeMain();
    virtual void deserializeMain(std::shared_ptr<ObjectMap>);

protected:
    double m_rotation = 0.0;
    sf::Vector2f m_origin;
    sf::Vector2f m_motion;
    sf::Vector2f m_scale = sf::Vector2f(1, 1);

private:
    sf::Vector2f m_position;
};

}
