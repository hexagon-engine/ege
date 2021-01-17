/*
*
*   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*
*      ,----  ,----  ,----
*      |      |      |
*      |----  | --,  |----
*      |      |   |  |
*      '----  '---'  '----
*
*     Framework Library for Hexagon
*
*    Copyright (c) Sppmacd 2020 - 2021
*
*    Permission is hereby granted, free of charge, to any person obtaining a copy
*    of this software and associated documentation files (the "Software"), to deal
*    in the Software without restriction, including without limitation the rights
*    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
*    copies of the Software, and to permit persons to whom the Software is
*    furnished to do so, subject to the following conditions:
*
*    The above copyright notice and this permission notice shall be included in all
*    copies or substantial portions of the Software.
*
*    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
*    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
*    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
*    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
*    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
*    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
*    SOFTWARE.
*
*   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*
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
    SceneObject2D(Scene2D& scene, const SceneObjectType& type)
    : SceneObject((Scene&)scene, type) {}

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

    enum RotationMode
    {
        Inherit, // Object angle is parent angle + own angle
        Lock     // Parent angle is ignored
    };

    void setRotationMode(RotationMode mode)
    {
        m_rotationMode = mode;
        setGeometryNeedUpdate();
    }
    RotationMode getRotationMode() { return m_rotationMode; }

    double getRotation() const;
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
    RotationMode m_rotationMode = RotationMode::Inherit;
    SceneObjectType* m_type = nullptr;

private:
    Vec2d m_position;
};

}
