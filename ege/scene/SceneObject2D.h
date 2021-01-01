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

class Scene2D;

class SceneObject2D : public SceneObject
{

public:
    SceneObject2D(Scene2D& owner, String typeId)
    : SceneObject((Scene&)owner, typeId) {}

    virtual void setPosition(Vec2d position)
    {
        if(m_position != position)
        {
            setMainChanged();
            setGeometryNeedUpdate();
        }
        m_position = position;
    }
    Vec2d getPosition() const;

    virtual void setMotion(Vec2d motion)
    {
        // We don't need to update geometry since motion doesn't
        // actually change anything in appearance of object. If
        // someone wants to update geometry, they should override
        // that function and manually call setGeometryNeedUpdate().

        if(m_motion != motion)
            setMainChanged();

        m_motion = motion;
    }
    Vec2d getMotion() const { return m_motion; }

    virtual sf::FloatRect getBoundingBox() const { return sf::FloatRect(m_position.x, m_position.y, 0.f, 0.f); }

    sf::FloatRect getBoundingBox(Vec2d pos)
    {
        Vec2d tmp = m_position;
        m_position = pos;
        sf::FloatRect rect = getBoundingBox();
        m_position = tmp;
        return rect;
    }

    double getRotation() const { return m_rotation; }
    void setRotation(double rotation)
    {
        m_rotation = rotation;
        setGeometryNeedUpdate();
    }

    Vec2d getOrigin() const { return m_origin; }
    void setOrigin(Vec2d origin)
    {
        m_origin = origin;
        setGeometryNeedUpdate();
    }

    Vec2d getScale() const { return m_scale;}
    void setScale(Vec2d scale)
    {
        m_scale = scale;
        setGeometryNeedUpdate();
    }

    // with collision check
    virtual bool moveTo(Vec2d pos, bool notify = true);

    // with collision check
    virtual bool flyTo(Vec2d pos, double time, std::function<double(double)> easing = AnimationEasingFunctions::linear);

    virtual void render(Renderer& renderer) const override;
    virtual void onUpdate(long long tickCounter) override;

    virtual std::shared_ptr<ObjectMap> serializeMain() const override;
    virtual bool deserializeMain(std::shared_ptr<ObjectMap>) override;

protected:
    double m_rotation = 0.0;
    Vec2d m_origin;
    Vec2d m_motion;
    Vec2d m_scale = {1, 1};

private:
    Vec2d m_position;
};

}
