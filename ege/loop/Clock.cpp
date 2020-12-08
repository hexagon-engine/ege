/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#include "Clock.h"

#include "EventLoop.h"

namespace EGE
{

MaxFloat Clock::restart()
{
    MaxFloat time = m_loop->time(m_startTime.getUnit());
    MaxFloat elapsed = time - m_startTime.getValue();
    m_startTime = { time, m_startTime.getUnit() };
    return elapsed;
}

MaxFloat Clock::getElapsedTime()
{
    MaxFloat time = m_loop->time(m_startTime.getUnit());
    return time - m_startTime.getValue();
}

}
