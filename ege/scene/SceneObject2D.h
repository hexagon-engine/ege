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

#define SCENEOBJECT2D_DEBUG 0

namespace EGE
{

class SceneObject2D : public SceneObject
{

public:
    SceneObject2D(EGE::Scene* owner, std::string typeId)
    : SceneObject(owner, typeId) {}

    virtual void setPosition(sf::Vector2f position)
    {
        if(m_position != position)
        {
            setMainChanged();
            m_geometryChanged = true;
        }
        m_position = position;
    }
    sf::Vector2f getPosition() const
    {
        return m_position;
    }
    virtual void setMotion(sf::Vector2f motion)
    {
        if(m_motion != motion)
        {
            setMainChanged();
            m_geometryChanged = true;
        }
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

    sf::FloatRect getBoundingBox(sf::Vector2f pos)
    {
        sf::Vector2f tmp = m_position;
        m_position = pos;
        sf::FloatRect rect = getBoundingBox();
        m_position = tmp;
        return rect;
    }

    // with collision check
    virtual bool moveTo(sf::Vector2f pos, bool notify = true);

    // with collision check
    virtual bool flyTo(sf::Vector2f pos, double time, std::function<double(double)> easing = AnimationEasingFunctions::linear);

    virtual void render(sf::RenderTarget& target) const;
    virtual void onUpdate(long long tickCounter);

    virtual std::shared_ptr<ObjectMap> serializeMain();
    virtual void deserializeMain(std::shared_ptr<ObjectMap>);

protected:
    virtual void updateGeometry() {}

    double m_rotation = 0.0;
    sf::Vector2f m_origin;
    sf::Vector2f m_motion;
    sf::Vector2f m_scale = sf::Vector2f(1, 1);
    bool m_geometryChanged = true;

private:
    sf::Vector2f m_position;
};

}
