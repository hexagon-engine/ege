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
#include <vector>

#define ANIMATION_DEBUG 0

namespace EGE
{

class Animation : public Timer
{
public:
    Animation(EventLoop* loop, Time duration, Timer::Mode mode = Timer::Mode::Limited)
    : Timer(loop, mode, duration) {}

    // time is value from 0 (start()) to 1 (start() + duration)
    // value can be everything
    void addKeyframe(double time, double value);

    bool isKeyframe(double time);

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
    double getValue(double time);

    void setEasingFunction(std::function<double(double)> ease)
    {
        m_ease = ease;
    }
    void setDelay(Time delay)
    {
        m_delay = delay;
    }
    Time getDelay()
    {
        return m_delay;
    }

private:
    std::vector<std::pair<double, double>> m_keyframes;
    std::function<double(double)> m_ease;
    bool m_sorted = false;
    Time m_delay = Time(0, Time::Unit::Ticks);
};

}
