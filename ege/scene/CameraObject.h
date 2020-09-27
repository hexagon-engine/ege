/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
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
    CameraObject(std::shared_ptr<EGE::Scene> parent)
    : m_parent(parent) {}

    CoordType getEyePosition() { return CoordType(); };

    void setScalingMode(ScalingMode mode)
    {
        m_scalingMode = mode;
    }

    ScalingMode getScalingMode()
    {
        return m_scalingMode;
    }

    void setFOV(float fov)
    {
        m_fov = fov;
    }

    float getFOV() const
    {
        return m_fov;
    }

    void setSize(CoordType size)
    {
        m_size = size;
    }

    CoordType getSize()
    {
        return m_size;
    }

protected:
    float m_fov = 0.f;
    CoordType m_size;
    std::shared_ptr<EGE::Scene> m_parent;
    ScalingMode m_scalingMode = ScalingMode::None;
};

}
