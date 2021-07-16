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

#include "EventHandler.h"

#include "AsyncTask.h"
#include "Timer.h"

#include <ege/debug/InspectorNode.h>

#include <map>
#include <memory>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

namespace EGE
{

class EventLoop : public InspectorNode
{
public:
    EGE_ENUM_YES_NO(TimerImmediateStart);

    EventLoop(InspectorNode* parent, String id = "EventLoop")
    : InspectorNode(parent, id) {}

    EventLoop(String id = "EventLoop")
    : InspectorNode(id) {}

    virtual ~EventLoop() { if(m_running) exit(0); }

    template<class EvtT = Event>
    class EventArray
    {
    public:
        void clear() { m_handlers.clear(); }

        template<class Evt = EvtT>
        EventArray<EvtT>& add(typename SimpleEventHandler<Evt>::Handler handler)
        {
            return addHandler<SimpleEventHandler<Evt>>(handler);
        }

        EventArray<EvtT>& remove(EventHandler& handler)
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

        EventArray<EvtT>& addHandler(SharedPtr<EventHandler> handler)
        {
            m_handlers.push_back(handler);
            return *this;
        }

        template<class Evt = EvtT, class... Args>
        EventResult fire(Args&&... args)
        {
            Evt event(args...);
            return fire(event);
        }

        EventResult fire(EvtT& event)
        {
            EventResult result = EventResult::Success;
            m_inEventHandler = true;
            for(auto& pr: m_handlers)
            {
                (bool&)result |= (bool)pr->handle(event);
            }
            m_inEventHandler = false;
            return result;
        }

    private:
        friend class EventLoop;

        SharedPtrVector<EventHandler> m_handlers;
        bool m_inEventHandler = false;
    };

    template<class EvtT>
    class LockingEventArray
    {
    public:
        LockingEventArray(EventArray<EvtT>& array, std::mutex& mutex)
        : m_array(array), m_lock(mutex) {}

        template<class Evt = EvtT>
        LockingEventArray<EvtT>& add(typename SimpleEventHandler<Evt>::Handler handler)
            { m_array.add(handler); return *this; }

        LockingEventArray<EvtT>& remove(EventHandler& handler)
            { m_array.remove(handler); return *this; }

        template<class EvtHandler, class... Args>
        LockingEventArray<EvtT>& addHandler(Args&&... args)
            { m_array.template addHandler<EvtHandler>(args...); return *this; }

        LockingEventArray<EvtT>& addHandler(SharedPtr<EventHandler> handler)
            { m_array.addHandler(handler); return *this; }

        template<class Evt = EvtT, class... Args>
        EventResult fire(Args&&... args)
            { return m_array.template fire<Evt>(args...); }

        EventResult fire(EvtT& event)
            { return m_array.fire(event); }

    private:
        EventArray<EvtT>& m_array;
        std::lock_guard<std::mutex> m_lock;
    };

    template<class Evt>
    LockingEventArray<Evt> events() { return LockingEventArray<Evt>((EventArray<Evt>&)events(Evt::type()), m_mutex); }

    template<class Evt>
    EventResult fire(Evt& evt) { return events<Evt>().fire(evt); }

    template<class Evt, class... Args>
    EventResult fire(Args&&... args) { return events<Evt>().fire(args...); }

    virtual void onTimerFinish(Timer*) {}
    virtual void onTimerTick(Timer*) {}

    virtual void addTimer(const std::string& name, SharedPtr<Timer> timer, TimerImmediateStart start = TimerImmediateStart::Yes);
    virtual std::vector<std::weak_ptr<Timer>> getTimers(const std::string& timer);
    virtual void removeTimer(const std::string& timer);
    virtual void onUpdate();
    virtual void deferredInvoke(std::function<void()> func);

    // get in-loop time in ticks or ms
    // it should be used ONLY for comparisions
    virtual double time(Time::Unit unit);

    virtual int run();

    virtual void exit(int exitCode = 0);
    bool isRunning() { return m_running; }
    long long getTickCount() { return m_ticks; }

    virtual void addSubLoop(SharedPtr<EventLoop> loop);
    virtual void removeSubLoop(EventLoop& loop);

    virtual void addAsyncTask(SharedPtr<AsyncTask> task, std::string name = "");
    virtual void removeAsyncTasks(std::string name = "");
    virtual std::vector<std::weak_ptr<AsyncTask>> getAsyncTasks(std::string name = "");

private:
    std::multimap<std::string, SharedPtr<AsyncTask>> m_asyncTasks;

protected:
    virtual void updateTimers();
    virtual void updateAsyncTasks();
    virtual void callDeferredInvokes();
    int m_exitCode = 0;
    SharedPtrVector<EventLoop> m_subLoops;

private:
    EventArray<Event>& events(Event::EventType type);

    int m_ticks = 0;
    bool m_running = true;
    std::multimap<std::string, SharedPtr<Timer>> m_timers;
    Map<Event::EventType, EventArray<Event>> m_eventHandlers;
    std::queue<std::function<void()>> m_deferredInvokes;
    std::mutex m_mutex;
};

}
