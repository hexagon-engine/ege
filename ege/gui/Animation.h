/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#pragma once

#include <ege/loop/EventLoop.h>
#include <ege/loop/Timer.h>
#include <vector>

#define ANIMATION_DEBUG 1

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
