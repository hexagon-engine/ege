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

#include "Timer.h"

#include "EventLoop.h"

#include <iostream>
#include <ege/main/Config.h>
#include <ege/util/PointerUtils.h>
#include <time.h>

namespace EGE
{

void Timer::start()
{
    m_startTime = m_loop->time(m_interval.getUnit());
    m_started = true;
    m_iterations = 0;
    m_remainingIterations = m_iterations + 1;
}
void Timer::stop()
{
    m_started = false;
}
Timer::Finished Timer::update()
{
    DUMP(TIMER_DEBUG, m_name);
    DUMP(TIMER_DEBUG, (long long)m_interval.getValue());
    DUMP(TIMER_DEBUG, m_started);
    DUMP(TIMER_DEBUG, (long long)m_startTime);
    DUMP(TIMER_DEBUG, m_interval.getUnit() == Time::Unit::Ticks);

    double time = m_loop->time(m_interval.getUnit());
    DUMP(TIMER_DEBUG, (long long)time);

    if(m_updateCallback)
        m_updateCallback(m_name, this);

    if(m_started && time >= m_startTime + m_interval.getValue())
    {
        m_iterations++;

        if(m_callback)
            m_callback(m_name, this);

        switch(m_mode)
        {
        case Mode::Limited:
            m_remainingIterations--;
            m_startTime = time;
            if(m_remainingIterations == 0)
                return Finished::Yes;
            break;
        case Mode::Infinite:
            m_startTime = time;
        default:
            break;
        }
    }
    return Finished::No;
}
Time Timer::getElapsedTime()
{
    return Time(m_loop->time(m_interval.getUnit()) - m_startTime, m_interval.getUnit());
}
void Timer::restart()
{
    start();
}

}

