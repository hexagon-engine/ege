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

#include <ege/scene/ParticleSystem2D.h>

namespace EGE
{

class PhysicalParticle : public Particle
{
public:
    Vec3d motion;
};

class PhysicalParticleSystemImpl
{
public:
    Vec3d realStartMotion() const;

    void setGravity(Vec3d gravity) { m_gravity = gravity; } // px/t^2
    void setStartMotion(Vec3d motion) { m_startMotion = motion; } // px/t
    void setStartMotionValueRandom(double value) { m_startMotionValueRnd = value; } // %
    void setStartMotionAngleRandom(double value) { m_startMotionAngleRnd = value; } // degrees

protected:
    Vec3d m_gravity;
    Vec3d m_startMotion;
    double m_startMotionValueRnd = 0;
    double m_startMotionAngleRnd = 0;
};

// PP must be derived from PhysicalParticle
template<class PP>
class PhysicalParticleSystem : public ParticleSystem2D<PP>, public PhysicalParticleSystemImpl
{
public:
    EGE_SCENEOBJECT("EGE::PhysicalParticleSystem")

    PhysicalParticleSystem(EGE::Scene& owner)
    : ParticleSystem2D<PP>(owner) {}

    virtual void onParticleSpawn(PP& particle) const override
    {
        particle.motion += realStartMotion();
    }

    virtual void onParticleUpdate(PP& particle) const override
    {
        // TODO: Collisions with other SceneObjects
        // TODO: Disturbations (wind?)
        particle.motion += m_gravity;

        // Update motion
        particle.position += particle.motion;
    }
};

using DefaultPhysicalParticleSystem = PhysicalParticleSystem<PhysicalParticle>;

}
