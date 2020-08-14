/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#include "Timer.h"

#include "EventLoop.h"

#include <iostream>
#include <ege/main/Config.h>
#include <time.h>

namespace EGE
{

void Timer::start()
{
    m_startTime = m_loop->time(m_interval.getUnit());
    m_started = true;
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

    if(m_started && time >= m_startTime + m_interval.getValue())
    {
        DBG(1, "run callback");
        m_iterations++;
        m_callback(m_name, this);
        switch(m_mode)
        {
        case Mode::Limited:
            m_remaining_iterations--;
            m_startTime = time;
            if(m_remaining_iterations == 0)
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

}

