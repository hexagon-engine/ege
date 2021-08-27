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
*    Copyright (c) Sppmacd 2021
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

#include <ege/core/Event.h>
#include <ege/core/EventHandler.h>
#include <ege/core/EventResult.h>

namespace EGE
{

template<class EvtT = Event>
class EventArray
{
public:
    EventArray() = default;
    EventArray(const EventArray&) = delete;
    EventArray(EventArray&&) = delete;

    void clear() { m_handlers.clear(); }

    template<class Evt = EvtT>
    EventArray<EvtT>& add(typename SimpleEventHandler<Evt>::Handler handler)
    {
        return addHandler<SimpleEventHandler<Evt>>(handler);
    }

    EventArray<EvtT>& remove(EventHandlerBase& handler)
    {
        ASSERT(!m_inEventHandler);
        for(size_t s = 0; s < m_handlers.size(); s++)
        {
            if(m_handlers[s].get() == &handler)
            {
                m_handlers.erase(m_handlers.begin() + s);
                return *this;
            }
        }
        return *this;
    }

    template<class EvtHandler, class... Args>
    EventArray<EvtT>& addHandler(Args&&... args)
    {
        return addHandler(make<EvtHandler>(args...));
    }

    EventArray<EvtT>& addHandler(SharedPtr<EventHandlerBase> const& handler)
    {
        m_handlers.push_back(handler);
        return *this;
    }

    EventResult fire(EvtT& event)
    {
        bool result = true;
        m_inEventHandler = true;
        for(auto& pr: m_handlers)
        {
            result &= (pr->doHandle(event) == EventResult::Success);
        }
        m_inEventHandler = false;
        return result ? EventResult::Success : EventResult::Failure;
    }

private:
    SharedPtrVector<EventHandlerBase> m_handlers;
    bool m_inEventHandler = false;
};

}
