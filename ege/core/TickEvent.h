#pragma once

#include <ege/core/Event.h>
#include <ege/util/Types.h>

namespace EGE
{

class TickEvent : public Event
{
public:
    EGE_EVENT("EGE::TickEvent");

    TickEvent(TickCount tickCount)
    : m_count(tickCount) {}

    TickCount count() const { return m_count; }

    virtual bool cancelable() const override { return false; }

private:
    TickCount m_count = 0;
};

}
