/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#pragma once

#include "SceneObject.h"

#include <ege/gui/AnimationEasingFunctions.h>
#include <ege/main/Config.h>
#include <ege/util/PointerUtils.h>
#include <functional>
#include <SFML/Graphics.hpp>

#define SCENEOBJECT2D_DEBUG 1

namespace EGE
{

class SceneObject2D : public SceneObject
{

public:
    SceneObject2D(std::shared_ptr<EGE::Scene> owner, std::string typeId)
    : SceneObject(owner, typeId) {}

    virtual void setPosition(sf::Vector2f position)
    {
        if(m_position != position)
        {
            setMainChanged();
            if(m_renderer)
                m_renderer->setGeometryNeedUpdate();
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
            if(m_renderer)
                m_renderer->setGeometryNeedUpdate();
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

    double getRotation() const
    {
        return m_rotation;
    }
    void setRotation(double rotation)
    {
        m_rotation = rotation;
    }

    sf::Vector2f getOrigin() const
    {
        return m_origin;
    }
    void setOrigin(sf::Vector2f origin)
    {
        m_origin = origin;
    }

    sf::Vector2f getScale() const
    {
        return m_scale;
    }
    void setScale(sf::Vector2f scale)
    {
        m_scale = scale;
    }

    // with collision check
    virtual bool moveTo(sf::Vector2f pos, bool notify = true);

    // with collision check
    virtual bool flyTo(sf::Vector2f pos, double time, std::function<double(double)> easing = AnimationEasingFunctions::linear);

    virtual void render(sf::RenderTarget& target, const RenderStates& states) const override;
    virtual void onUpdate(long long tickCounter) override;

    virtual std::shared_ptr<ObjectMap> serializeMain() const override;
    virtual bool deserializeMain(std::shared_ptr<ObjectMap>) override;

protected:
    double m_rotation = 0.0;
    sf::Vector2f m_origin;
    sf::Vector2f m_motion;
    sf::Vector2f m_scale = sf::Vector2f(1, 1);

private:
    sf::Vector2f m_position;
};

}
