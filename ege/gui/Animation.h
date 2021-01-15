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

#include <ege/loop/EventLoop.h>
#include <ege/loop/Timer.h>
#include <ege/util/Color.h>
#include <ege/util/Vector.h>
#include <vector>

#define ANIMATION_DEBUG 0

namespace EGE
{

// T must have:
//  operator+(const T&, const T&)
//  operator-(const T&, const T&)
//  operator*(const T&, double)
//  operator/(const T&, double)
//
// Example compatible types:
//  * all primitives
//  * EGE::Vec2d
//
template<class T>
class Animation : public Timer
{
public:
    typedef T Type;

    Animation(EventLoop& loop, Time duration, Timer::Mode mode = Timer::Mode::Limited)
    : Timer(loop, mode, duration) {}

    // time is value from 0 (start()) to 1 (start() + duration)
    // value can be everything
    void addKeyframe(MaxFloat time, T value)
    {
        m_keyframes.push_back(std::make_pair(time, value));
    }

    bool isKeyframe(MaxFloat time)
    {
        for(auto pr: m_keyframes)
            if(pr.first == time)
                return true;

        return false;
    }

    // Returns value based on time and current easing mode (default - linear)
    // Example: (# = keyframe)
    // ^ value
    // |                   #*
    // |                  *|  *#*******
    // |                 * |   |
    // |*******#****    *  |   |
    // |       |    ***#   |   |
    // |       |       |   |   |
    // 0------.25-----.5---#--.75-----1---> time
    T getValue(MaxFloat time)
    {
        ASSERT(!m_keyframes.empty());
        std::pair<MaxFloat, T> previous;

        if(m_iterations > 1)
             previous = m_keyframes.back();

        // ensure that keyframes are sorted!
        if(!m_sorted)
        {
            std::sort(m_keyframes.begin(), m_keyframes.end(), [](std::pair<MaxFloat, T> _L, std::pair<MaxFloat, T> _R)->bool {
                return _L.first < _R.first;
            });

            // add "zero frame" and "last frame" if it doesn't exist
            if(!isKeyframe(0.0)) m_keyframes.insert(m_keyframes.begin(), std::make_pair(0.0, m_keyframes.front().second));
            if(!isKeyframe(1.0)) m_keyframes.insert(m_keyframes.end(), std::make_pair(1.0, m_keyframes.back().second));

            m_sorted = true;
        }
        DUMP(ANIMATION_DEBUG, time);

        // clamp animation if it won't be repeated
        if(m_remainingIterations == 1)
            time = std::min(1.0L, std::max(0.0L, time));

        while(time > 1.0)
            time--;

        while(time < 0.0)
            time++;

        for(auto keyframe: m_keyframes)
        {
            if(keyframe.first >= time)
            {
                MaxFloat timeDiff = (keyframe.first - previous.first);

                if(timeDiff == 0.0)
                    return (keyframe.second + previous.second) / 2.0;

                MaxFloat td2 = (time - previous.first);
                MaxFloat timeFactor = td2 / timeDiff;

                if(m_ease)
                    timeFactor = m_ease(timeFactor);
                return (keyframe.second - previous.second) * (double)timeFactor + previous.second;
            }
            previous = keyframe;
        }
        return T();
    }

    void setEasingFunction(std::function<MaxFloat(MaxFloat)> ease) { m_ease = ease; }
    void setDelay(Time delay) { m_delay = delay; }
    Time getDelay() { return m_delay; }

private:
    PairVector<MaxFloat, T> m_keyframes;
    std::function<MaxFloat(MaxFloat)> m_ease;
    bool m_sorted = false;
    Time m_delay = Time(0, Time::Unit::Ticks);
};

typedef Animation<MaxFloat> NumberAnimation;
typedef Animation<Vec2d> Vec2Animation;
typedef Animation<ColorRGBA> RGBAnimation;

}
