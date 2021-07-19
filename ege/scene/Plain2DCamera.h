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

#include "Camera.h"
#include "SceneObject.h"

#include <ege/gfx/RenderStates.h>
#include <SFML/Graphics.hpp>

#define CAMERA_DEBUG 0

namespace EGE
{

enum class ScalingMode
{
    None, // the default, no scaling
    Centered, // the camera display is centered on scene widget, but not scaled
    Fit, // screen is scaled to specified SIZE, but keep proportions
    XLocked, // the x coord is always locked to specified FOV value
    YLocked, // the y coord is always locked to specified FOV value
    Scaled, // the view is forced to be scaled to specified SIZE ("manual mode")
};

class Plain2DCamera : public Camera
{
public:
    EGE_SCENEOBJECT("EGE::Plain2DCamera");

    Plain2DCamera(EGE::Scene& scene)
    : Camera(scene) {}

    void setScalingMode(ScalingMode mode) { m_scalingMode = mode; }
    ScalingMode getScalingMode() { return m_scalingMode; }

    void setFOV(float fov) { m_fov = fov; }
    float getFOV() const { return m_fov; }

    void setDisplaySize(Vec2d size) { m_displaySize = size; }
    Vec2d getDisplaySize() const { return m_displaySize; }

    void setZoom(float zoom) { ASSERT(zoom != 0); m_zoom = zoom; }
    float getZoom() const { return m_zoom; }

    // TODO: What if object dies?
    void setFollowObject(EGE::SceneObject const* object) { m_following = object; }

    void setEyePosition(Vec2d position) { setPosition(position); }
    Vec2d getEyePosition() const { return m_following ? m_following->getPosition().toVec2d() : getPosition().toVec2d(); }

    virtual void applyTransform(Renderer& renderer) const override;

    virtual Vec2d mapToScreenCoords(Renderer& renderer, Vec3d scene) const override;
    virtual Vec3d mapToSceneCoords(Renderer& renderer, Vec2d screen) const override;

private:
    sf::View getView(sf::View parentView) const;

    float m_fov = 0.f;
    float m_zoom = 1.f;
    Vec2d m_displaySize;
    ScalingMode m_scalingMode = ScalingMode::None;
    EGE::SceneObject const* m_following = nullptr;
};

}

