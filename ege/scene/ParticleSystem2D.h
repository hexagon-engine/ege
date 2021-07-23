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

#include <ege/gfx/RenderStates.h>
#include <list>

namespace EGE
{

template<class P>
class ParticleSystem2D;

class Particle
{
public:
    void setDead() { ttl = 0; }

    Vec3d position;
    unsigned ttl = 0;
};

class ParticleSystemImpl : public SceneObject
{
public:
    ParticleSystemImpl(Scene& owner)
    : SceneObject(owner) {}

    virtual void onUpdate(TickCount) override;

    // Chance that the particle will be spawned in current onUpdate call.
    // If val == 1, particle will be spawned every tick.
    // If val > 1, 'val' particles will be spawned.
    // If val <= 0, CRASH().
    void setSpawnChance(double val) { ASSERT(val > 0); m_spawnChance = val; }
    void setParticleLifeTime(unsigned ttl) { m_particleTTL = ttl; }

    RectD getSpawnRect() const { return m_spawnRect; }
    void setSpawnRect(RectD rect) { m_spawnRect = rect; }

    virtual void spawnParticle(Vec3d relativePosition) = 0;

    void spawnParticles(size_t count);
    void setEnabled(bool enabled) { m_enabled = enabled; }

protected:
    RectD m_spawnRect;
    double m_spawnChance = 1.0;
    unsigned m_particleTTL = 60; // 1s
    bool m_enabled = true;

    virtual void updateParticles() = 0;
    Vec3d randomPosition();
};

/*
    Particle System

    The Particle System is an Object which have many simple Sub Objects.
    The sub objects are rendered and updated in a single way.

    P must be derived from Particle.
*/
template<class P = Particle>
class ParticleSystem2D : public ParticleSystemImpl
{
public:
    using ParticleType = P;

    ParticleSystem2D(Scene& owner)
    : ParticleSystemImpl(owner) {}

    virtual void render(Renderer& renderer) const override
    {
        SceneObject::render(renderer);
        renderParticles(m_particles, renderer);
    }

    virtual void spawnParticle(Vec3d relativePosition) override
    {
        ParticleType particle;
        particle.position = randomPosition() + relativePosition;
        particle.ttl = m_particleTTL;

        onParticleSpawn(particle);

        m_particles.push_back(std::move(particle));
    }

    virtual RectD getBoundingBox() const override { return m_spawnRect; }

    void killAllParticles() { m_particles.clear(); }
    size_t getParticleCount() const { return m_particles.size(); }

private:
    virtual void updateParticles() override
    {
        for(auto it = m_particles.begin(); it != m_particles.end();)
        {
            auto current = it;
            auto next = ++it;

            ParticleType& particle = *current;
            particle.ttl--;
            onParticleUpdate(particle);

            if(particle.ttl <= 0)
            {
                m_particles.erase(current);
                it = next;
            }
        }
    }

    // TODO: Find a way to move it to Particle class
    virtual void onParticleUpdate(ParticleType&) const {}
    virtual void onParticleSpawn(ParticleType&) const {}

    virtual void renderParticles(const std::list<ParticleType>&, Renderer&) const {}
    std::list<ParticleType> m_particles;
};

class DefaultParticleSystem2D : public ParticleSystem2D<Particle>
{
public:
    EGE_SCENEOBJECT("EGE::DefaultParticleSystem2D")

    DefaultParticleSystem2D(Scene& owner)
    : ParticleSystem2D<Particle>(owner) {}
};

}
