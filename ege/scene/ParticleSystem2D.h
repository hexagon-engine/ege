/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#pragma once

#include "SceneObject2D.h"

#include <ege/gfx/RenderStates.h>
#include <list>

namespace EGE
{

/*
    Particle System

    The Particle System is an Object which have many simple Sub Objects.
    The sub objects are rendered and updated in a single way.
*/
class ParticleSystem2D : public SceneObject2D
{
public:
    class UserData {};

    class Particle
    {
    public:
        Particle(ParticleSystem2D* _system)
        : system(_system) {}

        void update();
        void setDead() { ttl = 0; }

        sf::Vector2f position;
        unsigned ttl = 0;
        ParticleSystem2D* system;

        // TODO: it should be better a flat pointer (memory performance)
        std::unique_ptr<UserData> userData = nullptr;
    };

    friend class Particle;

    ParticleSystem2D(std::shared_ptr<EGE::Scene> owner, sf::FloatRect spawnRect)
    : SceneObject2D(owner, "EGE::ParticleSystem2D"), m_spawnRect(spawnRect) {}

    virtual sf::FloatRect getBoundingBox() const
    {
        return m_spawnRect;
    }

    sf::FloatRect getSpawnRect() const
    {
        return m_spawnRect;
    }

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
    sf::Vector2f randomPosition();

    sf::FloatRect m_spawnRect;
    double m_spawnChance = 1.0;
    unsigned m_particleTTL = 60; // 1s
    std::function<void(Particle&)> m_particleUpdater;
    std::function<void(Particle&)> m_particleOnSpawn;
    std::function<void(const std::list<Particle>&, Renderer&)> m_particleRenderer;

    std::list<Particle> m_particles;
};

}
