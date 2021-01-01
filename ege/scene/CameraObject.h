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

#include "Scene.h"

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

// requirements for CoordType:
//  must have default constructor public
template<class CoordType>
class CameraObject
{
public:
    CameraObject(EGE::Scene& parent)
    : m_parent(parent) {}

    CoordType getEyePosition() { return CoordType(); };
    void setScalingMode(ScalingMode mode) { m_scalingMode = mode; }
    ScalingMode getScalingMode() { return m_scalingMode; }
    void setFOV(float fov) { m_fov = fov; }

    float getFOV() const { return m_fov; }
    void setSize(CoordType size) { m_size = size; }
    CoordType getSize() { return m_size; }

    float getZoom() const { return m_zoom; }
    void setZoom(float zoom) { ASSERT(zoom != 0); m_zoom = zoom; }

    void setFollowObject(std::shared_ptr<EGE::SceneObject> object) { m_following = object; }

protected:
    float m_fov = 0.f;
    float m_zoom = 1.f;
    CoordType m_size;
    EGE::Scene& m_parent;
    ScalingMode m_scalingMode = ScalingMode::None;
    std::shared_ptr<EGE::SceneObject> m_following;
};

}
