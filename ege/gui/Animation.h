/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#pragma once

#include <ege/loop/EventLoop.h>
#include <ege/loop/Timer.h>
#include <vector>

namespace EGE
{

class Animation : public Timer
{
public:
    Animation(EventLoop* loop, Time duration)
    : Timer(loop, Timer::Mode::Limited, duration) {}

    // time is value from 0 (start()) to 1 (start() + duration)
    // value can be everything
    void addKeyframe(double time, double value);

    // Returns value based on time and current easing mode (default - linear)
    // Example: (* = keyframe)
    // ^ value
    // |                   **
    // |                  *|  **
    // |                 * |   |*
    // |      ******    *  |   | **
    // |    ** |    ****   |   |   *
    // |  **   |       |   |   |     *
    // 0**----.25-----.5---#--.75-----1---> time
    double getValue(double time);

    void setEasingMode()
    {
        DBG(1, "not implemented");
        ASSERT(false);
    }

private:
    std::vector<std::pair<double, double>> m_keyframes;
};

}
