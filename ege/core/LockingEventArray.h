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

#include <ege/core/EventArray.h>

#include <mutex>

namespace EGE
{

template<class EvtT>
class LockingEventArray
{
public:
    explicit LockingEventArray(std::mutex& mutex, EventArray<EvtT>& array)
    : m_lock(mutex), m_array(array) {}

    LockingEventArray(const LockingEventArray&) = delete;
    LockingEventArray(LockingEventArray&&) = default;

    template<class Evt = EvtT>
    LockingEventArray<EvtT>& add(typename SimpleEventHandler<Evt>::Handler&& handler)
        { m_array.add(std::move(handler)); return *this; }

    LockingEventArray<EvtT>& remove(EventHandlerBase& handler)
        { m_array.remove(handler); return *this; }

    template<class EvtHandler, class... Args>
    LockingEventArray<EvtT>& addHandler(Args&&... args)
        { m_array.template addHandler<EvtHandler>(std::forward<Args>(args)...); return *this; }

    LockingEventArray<EvtT>& addHandler(UniquePtr<EventHandlerBase>&& handler)
        { m_array.addHandler(std::move(handler)); return *this; }

    template<class Evt = EvtT, class... Args>
    EventResult fire(Args&&... args)
        { return m_array.template fire<Evt>(std::forward<Args>(args)...); }

    EventResult fire(EvtT& event)
        { return m_array.fire(event); }

private:
    std::unique_lock<std::mutex> m_lock;
    EventArray<EvtT>& m_array;
};

}

