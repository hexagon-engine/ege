/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#pragma once

#include "Event.h"

namespace EGE
{

class TimerEvent : public Event
{
public:
    enum Type
    {
        Finish,
        Start,
        Tick
    };

    TimerEvent(Type type, Timer* timer)
    : m_timer(timer)
    , m_type(type)
    {
        ASSERT(timer);
    }

    Timer* getTimer() { return m_timer; }
    Type getEventType() { return m_type; }
private:
    Timer* m_timer;
    Type m_type;
};

}
