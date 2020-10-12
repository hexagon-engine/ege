/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#include "ParticleSystem2D.h"

#include <cstdlib>

namespace EGE
{

void ParticleSystem2D::render(sf::RenderTarget& target) const
{
    SceneObject2D::render(target);
    for(auto& particle: m_particles)
    {
        if(m_particleRenderer)
            m_particleRenderer(particle, target);
    }
}

void ParticleSystem2D::onUpdate(long long tickCounter)
{
    SceneObject2D::onUpdate(tickCounter);

    // update existing particles
    for(size_t i = 0; i < m_particles.size(); i++)
    {
        Particle& particle = m_particles[i];
        particle.update();

        if(particle.dead)
        {
            m_particles.erase(m_particles.begin() + i);
            i--;
            break;
        }
    }

    // spawn new particles
    double val = rand() % 1024 / 1024.0;
    if(val < m_spawnChance)
    {
        Particle particle(this);
        particle.position = randomPosition();
        particle.ttl = m_particleTTL;
        m_particles.push_back(particle);
    }
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
    if(ttl <= 0)
        dead = true;

    if(system->m_particleUpdater)
        system->m_particleUpdater(*this);
}

}
