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

#include <ege/util.h>
#include <SFML/Audio.hpp>

namespace EGE
{

class Sound;

class SoundInstance
{
public:
    virtual ~SoundInstance() = default;

    SoundInstance(Sound& sound)
    : m_sound(sound) {}

    virtual bool finished() = 0;

    virtual void pause() = 0;
    virtual void stop() = 0;

    // volume is in range 0-1
    virtual void setVolume(float volume) = 0;

    const Sound& sound() const { return m_sound; }

private:
    Sound& m_sound;
};

class Sound
{
public:
    virtual ~Sound() = default;

    virtual SharedPtr<SoundInstance> play() = 0;

    constexpr bool isError() const { return m_error; }

protected:
    void setError() { m_error = true; }

private:
    bool m_error = false;
};

class SFMLSound;

class SFMLSoundInstance : public SoundInstance
{
public:
    SFMLSoundInstance(SFMLSound& sound);

    virtual bool finished() override;

    virtual void pause() override;
    virtual void stop() override;

    // volume is in range 0-1
    virtual void setVolume(float volume) override;

private:
    sf::Sound m_sfSound;
};

class SFMLSound : public Sound
{
public:
    SFMLSound(std::string fileName);

    virtual SharedPtr<SoundInstance> play() override;

    const sf::SoundBuffer& buffer() const { return m_buffer; }

private:
    sf::SoundBuffer m_buffer;
};

}
