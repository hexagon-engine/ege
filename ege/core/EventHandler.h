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
#include "ege/util/Vector.h"

#include <ege/main/Config.h>
#include <ege/util/PointerUtils.h>
#include <functional>

namespace EGE
{

class EventHandlerBase
{
public:
    virtual ~EventHandlerBase() = default;
    virtual EventResult doHandle(Event& event) = 0;
};

template<class T>
class EventHandler : public EventHandlerBase
{
public:
    using EventType = T;

    virtual Event::EventType type() const { return EventType::type(); }
    virtual EventResult handle(EventType& event) = 0;

    virtual EventResult doHandle(Event& event) override
    {
        // FIXME: Maybe it's possible to do it when registering handlers to Component?
        ASSERT(instanceof(&event, EventType));
        return handle(static_cast<EventType&>(event));
    }
};

template<class T>
class SimpleEventHandler : public EventHandler<T>
{
public:
    typedef std::function<EventResult(T&)> Handler;

    explicit SimpleEventHandler(Handler handler)
        : m_handler(handler)
    {
        ASSERT_WITH_MESSAGE(m_handler, "Missing handler in SimpleEventHandler");
    }

    virtual EventResult handle(T& event) override
    {
        ASSERT(m_handler);
        return m_handler(event);
    }

private:
    Handler m_handler;
};

}
