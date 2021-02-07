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

#pragma once

#include "Event.h"
#include "EventResult.h"
#include "EventCast.h"

#include <ege/main/Config.h>
#include <ege/util/PointerUtils.h>
#include <functional>

namespace EGE
{

class EventHandler
{
public:
    virtual Event::EventType type() = 0;
    virtual EventResult handle(Event& event) { (void)event; return EventResult::Failure; }
};

template<class Evt>
class SimpleEventHandler : public EventHandler
{
public:
    typedef std::function<EventResult(Evt&)> Handler;

    explicit SimpleEventHandler(Handler handler)
        : m_handler(handler)
    {}

    virtual Event::EventType type() { return Evt::type(); }

    virtual EventResult handle(Event& event) override
    {
        auto event2 = dynamic_cast<Evt*>(&event);
        ASSERT(event2);
        if(m_handler)
            return m_handler(*event2);
        CRASH_WITH_MESSAGE("You need to give a handler or override `handle' function");
    }

private:
    Handler m_handler;
};

}
