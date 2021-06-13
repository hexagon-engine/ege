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

/*
    Particle System

    The Particle System is an Object which have many simple Sub Objects.
    The sub objects are rendered and updated in a single way.
*/
class ParticleSystem2D : public SceneObject
{
public:
    EGE_SCENEOBJECT("EGE::ParticleSystem2D");

    class UserData {};

    class Particle
    {
    public:
        Particle(ParticleSystem2D& _system)
        : system(_system) {}

        void update();
        void setDead() { ttl = 0; }

        Vec3d position;
        unsigned ttl = 0;
        ParticleSystem2D& system;

        // TODO: it should be better a flat pointer (memory performance)
        std::unique_ptr<UserData> userData = nullptr;
    };

    friend class Particle;

    ParticleSystem2D(Scene& owner)
    : SceneObject(owner) {}

    virtual RectD getBoundingBox() const override { return m_spawnRect; }

    RectD getSpawnRect() const { return m_spawnRect; }
    void setSpawnRect(RectD rect) { m_spawnRect = rect; }

    virtual void render(Renderer& renderer) const override;
    virtual void onUpdate(long long tickCounter) override;

    void setParticleUpdater(std::function<void(Particle&)> func) { m_particleUpdater = func; }
    void setParticleOnSpawn(std::function<void(Particle&)> func) { m_particleOnSpawn = func; }
    void setParticleRenderer(std::function<void(const std::list<Particle>&, Renderer&)> func) { m_particleRenderer = func; }

    // Chance that the particle will be spawned in current onUpdate call.
    // If val == 1, particle will be spawned every tick.
    // If val > 1, 'val' particles will be spawned.
    // If val <= 0, CRASH().
    void setSpawnChance(double val) { ASSERT(val > 0); m_spawnChance = val; }
    void setParticleLifeTime(unsigned ttl) { m_particleTTL = ttl; }

    void spawnParticle();

private:
    Vec2d randomPosition();

    RectD m_spawnRect;
    double m_spawnChance = 1.0;
    unsigned m_particleTTL = 60; // 1s
    std::function<void(Particle&)> m_particleUpdater;
    std::function<void(Particle&)> m_particleOnSpawn;
    std::function<void(const std::list<Particle>&, Renderer&)> m_particleRenderer;

    std::list<Particle> m_particles;
};

}
