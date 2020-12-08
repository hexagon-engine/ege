/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#pragma once

#include <ege/main/Config.h>
#include <ege/util/PointerUtils.h>
#include <ege/util/Time.h>
#include <functional>
#include <string>

#define TIMER_DEBUG 0

namespace EGE
{

class EventLoop;

class Clock
{
public:
    EGE_ENUM_YES_NO(Finished);

    Clock(EventLoop* loop, Time::Unit unit = Time::Unit::Seconds)
    : m_loop(loop), m_startTime(0, unit) { restart(); }

    // returns elapsed time
    MaxFloat restart();
    MaxFloat getElapsedTime();

protected:
    EventLoop* m_loop;
    Time m_startTime {};
};

}

