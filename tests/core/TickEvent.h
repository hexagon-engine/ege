#pragma once

#include <ege/core.h>

struct TickEvent : public EGE::Event
{
    long long m_tickCount;

    TickEvent(long long tickCount)
        : m_tickCount(tickCount)
    {}

    EGE_EVENT("TickEvent");
};
