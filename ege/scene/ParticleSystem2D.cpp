/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#include "ParticleSystem2D.h"
#include "Scene.h"

#include <cstdlib>

namespace EGE
{

void ParticleSystem2D::render(Renderer& renderer) const
{
    SceneObject2D::render(renderer);
    if(m_particleRenderer)
        m_particleRenderer(m_particles, renderer);
}

void ParticleSystem2D::onUpdate(long long tickCounter)
{
    if(!getOwner()->isHeadless()) getOwner()->getLoop()->getProfiler()->startSection("particleSystem");
    SceneObject2D::onUpdate(tickCounter);

    // update existing particles
    if(!getOwner()->isHeadless()) getOwner()->getLoop()->getProfiler()->startSection("update");
    for(auto it = m_particles.begin(); it != m_particles.end();)
    {
        auto current = it;
        auto next = ++it;

        Particle& particle = *current;
        particle.update();

        if(particle.ttl <= 0)
        {
            m_particles.erase(current);
            it = next;
        }
    }

    // spawn new particles
    if(!getOwner()->isHeadless()) getOwner()->getLoop()->getProfiler()->endStartSection("spawn");
    if(m_spawnChance == 1)
        spawnParticle();
    else if(m_spawnChance > 1)
    {
        for(size_t s = 0; s < m_spawnChance; s++)
            spawnParticle();
    }
    else
    {
        double val = rand() % 1024 / 1024.0;
        if(val < m_spawnChance)
            spawnParticle();
    }
    if(!getOwner()->isHeadless()) getOwner()->getLoop()->getProfiler()->endSection();

    if(!getOwner()->isHeadless()) getOwner()->getLoop()->getProfiler()->endSection();
}

sf::Vector2f ParticleSystem2D::randomPosition()
{
    int rand1 = rand() % 1024;
    int rand2 = rand() % 1024;

    float randSize1 = rand1 / 1024.f * m_spawnRect.width;
    float randSize2 = rand2 / 1024.f * m_spawnRect.height;

    float val1 = randSize1 + m_spawnRect.left;
    float val2 = randSize2 + m_spawnRect.top;

    return sf::Vector2f(val1, val2);
}

void ParticleSystem2D::Particle::update()
{
    ttl--;

    if(system->m_particleUpdater)
        system->m_particleUpdater(*this);
}

void ParticleSystem2D::spawnParticle()
{
    Particle particle(this);
    particle.position = randomPosition();
    particle.ttl = m_particleTTL;

    if(m_particleOnSpawn)
        m_particleOnSpawn(particle);

    m_particles.push_back(std::move(particle));
}

}
