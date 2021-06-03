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

#include "Sound.h"

#include <ege/core/EventLoop.h>

namespace EGE
{

class SoundManager : public EventLoop
{
public:
    EGE_ENUM_YES_NO(ChangeVolumeForAllSounds);

    SoundManager(String id = "SoundManager")
    : EventLoop(id) {}

    // TODO: Support loading from memory etc.

    SharedPtr<Sound> loadSoundFromFile(std::string name);

    void playSound(Sound& sound);
    void pauseSound(Sound& sound);
    void stopSound(Sound& sound);
    
    // TODO: Support some channels to allow setting volume on them separately
    // volume is in range 0-1
    void setVolume(float volume, ChangeVolumeForAllSounds updateVolume = ChangeVolumeForAllSounds::Yes);

    virtual void onUpdate() override;

private:
    float m_volume = 1.f;
    SharedPtrStringMap<Sound> m_loadedSounds;
    SharedPtrVector<SoundInstance> m_playedSounds;
};

}
