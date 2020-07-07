/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#pragma once

#include <main/Config.h>
#include "Event.h"
#include "EventResult.h"
#include "EventCast.h"

namespace EGE
{

class EventHandler
{
public:
    virtual EventResult handle(Event& event) { (void)event; return EventResult::Failure; }
};

template<class EventClassType>
class SimpleEventHandler : public EventHandler
{
public:
    typedef EventResult(*Handler)(EventClassType& event);

    explicit SimpleEventHandler(Handler handler)
        : m_handler(handler)
    {}

    virtual EventResult handle(Event& event) override
    {
        EventClassType* event2 = (EventClassType*)(&event);
        ASSERT(event2);
        return m_handler(*event2);
    }

private:
    Handler m_handler;
};

}
