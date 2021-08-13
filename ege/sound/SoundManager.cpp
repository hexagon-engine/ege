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

#include "SoundManager.h"

#include <ege/debug/Logger.h>

namespace EGE
{

SharedPtr<Sound> SoundManager::loadSoundFromFile(std::string name)
{
    auto sound = make<SFMLSound>(name);
    ASSERT(sound);
    if(sound->isError())
        return nullptr;
    m_loadedSounds.insert(std::make_pair(name, sound));
    return sound;
}

void SoundManager::playSound(Sound& sound)
{
    auto soundInstance = sound.play();
    soundInstance->setVolume(m_volume);
    m_playedSounds.push_back(soundInstance);
}

void SoundManager::pauseSound(Sound& sound)
{
    for(auto& soundInstance: m_playedSounds)
    {
        if(&soundInstance->sound() == &sound)
            soundInstance->pause();
    }
}

void SoundManager::stopSound(Sound& sound)
{
    for(auto& soundInstance: m_playedSounds)
    {
        if(&soundInstance->sound() == &sound)
            soundInstance->stop();
    }
}

// volume is in range 0-1
void SoundManager::setVolume(float volume, ChangeVolumeForAllSounds updateVolume)
{
    m_volume = volume;
    if(updateVolume == ChangeVolumeForAllSounds::Yes)
    {
        for(auto& soundInstance: m_playedSounds)
            soundInstance->setVolume(m_volume);
    }
}

void SoundManager::onTick()
{
    // Erase finished sounds.
    auto it = std::remove_if(m_playedSounds.begin(), m_playedSounds.end(), [](SharedPtr<SoundInstance> inst) {
        return inst->finished();
    });
    if(it != m_playedSounds.end())
        m_playedSounds.erase(it);
}

}
