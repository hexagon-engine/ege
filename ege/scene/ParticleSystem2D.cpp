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

#include "ParticleSystem2D.h"
#include "Scene.h"

#include <cstdlib>

namespace EGE
{

void ParticleSystemImpl::onTick()
{
    if(!getOwner().isHeadless())
        getOwner().getLoop()->getProfiler()->startSection("particleSystem");

    SceneObject::onTick();

    // update existing particles
    if(!getOwner().isHeadless())
        getOwner().getLoop()->getProfiler()->startSection("update");

    updateParticles();

    // spawn new particles
    if(m_enabled)
    {
        if(!getOwner().isHeadless())
            getOwner().getLoop()->getProfiler()->endStartSection("spawn");

        auto position = getPosition();
        if(m_spawnChance == 1)
            spawnParticle(position);
        else if(m_spawnChance > 1)
        {
            for(size_t s = 0; s < m_spawnChance; s++)
                spawnParticle(position);
        }
        else
        {
            double val = rand() % 1024 / 1024.0;
            if(val < m_spawnChance)
                spawnParticle(position);
        }
    }
    if(!getOwner().isHeadless())
        getOwner().getLoop()->getProfiler()->endSection();

    if(!getOwner().isHeadless())
        getOwner().getLoop()->getProfiler()->endSection();
}

void ParticleSystemImpl::spawnParticles(size_t count)
{
    for(size_t s = 0; s < count; s++)
        spawnParticle(getPosition());
}

Vec3d ParticleSystemImpl::randomPosition()
{
    // TODO: Support 3D spawn ranges
    int rand1 = rand() % 1024;
    int rand2 = rand() % 1024;

    float randSize1 = rand1 / 1024.f * m_spawnRect.size.x;
    float randSize2 = rand2 / 1024.f * m_spawnRect.size.y;

    float val1 = randSize1 + m_spawnRect.position.x;
    float val2 = randSize2 + m_spawnRect.position.y;

    return Vec3d(val1, val2, 0);
}

}
