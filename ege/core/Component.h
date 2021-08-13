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

#include "AsyncTask.h"
#include "EventArray.h"
#include "EventHandler.h"
#include "LockingEventArray.h"
#include "Timer.h"

#include <ege/debug/InspectorNode.h>
#include <ege/debug/Logger.h>
#include <ege/debug/Profiler.h>

#include <map>
#include <memory>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

namespace EGE
{

class ComponentBase : public InspectorNode
{
public:
    ComponentBase(ComponentBase& parent, String id = "ComponentBase")
    : InspectorNode(&parent, id) {}

    ComponentBase(String id = "ComponentBase")
    : InspectorNode(id) {}

    EGE_ENUM_YES_NO(TimerImmediateStart);

    template<class Evt>
    LockingEventArray<Evt> events()
    {
        return LockingEventArray<Evt>(m_eventHandlersMutex, (EventArray<Evt>&)m_eventHandlers[Evt::type()]);
    }

    template<class Evt, class... Args>
    EventResult fire(Args&&... args) { Evt event(std::forward<Args>(args)...); return fireEvent(event); }

    virtual EventResult fireEvent(Event& event) { return events(event.getType()).fire(event); }

    void addTimer(std::string const& name, SharedPtr<Timer> timer, TimerImmediateStart start = TimerImmediateStart::Yes);
    std::vector<std::weak_ptr<Timer>> getTimers(const std::string& timer);
    void removeTimer(std::string const& timer);
    void deferredInvoke(std::function<void()> func);

    // get in-loop time in ticks or ms
    // it should be used ONLY for comparisions
    double time(Time::Unit unit);

    virtual void onUpdate();

    virtual void exit(int exitCode = 0);

    void addAsyncTask(SharedPtr<AsyncTask> task, std::string name = "");
    void removeAsyncTasks(std::string name = "");
    std::vector<std::weak_ptr<AsyncTask>> getAsyncTasks(std::string name = "");

    Profiler* getProfiler() { return m_profiler; }

    virtual EventResult onLoad() { return EventResult::Success; }
    virtual void onTick() {}
    virtual EventResult onFinish(int /*exitCode*/) { return EventResult::Success; }

    bool isRunning() const { return m_running; }
    TickCount getTickCount() const { return m_ticks; }
    int getExitCode() const { return m_exitCode.load(); }

    // NOTE: Use it if you are sure that parent is of type U!
    template<class U>
    U* getParent() const { return static_cast<U*>(isnParent()); }

    template<class U>
    U* tryGetParent() const { return dynamic_cast<U*>(isnParent()); }

protected:
    Profiler* m_profiler = nullptr;
    virtual void onProfilerResults(Profiler const&) {}

    virtual void onExitInternal() {}
    virtual EventResult onFinishInternal(int exitCode) { return onFinish(exitCode); }

    EventArray<Event>& events(Event::EventType type);

private:
    std::atomic<TickCount> m_ticks = 0;
    std::atomic<int> m_exitCode = 0;
    std::atomic<bool> m_running = true;

    virtual void updateTimers();
    virtual void updateAsyncTasks();
    virtual void callDeferredInvokes();

    std::multimap<std::string, SharedPtr<AsyncTask>> m_asyncTasks;
    std::mutex m_asyncTasksMutex;

    std::multimap<std::string, SharedPtr<Timer>> m_timers;
    std::mutex m_timersMutex;

    Map<Event::EventType, EventArray<Event>> m_eventHandlers;
    std::mutex m_eventHandlersMutex;

    std::queue<std::function<void()>> m_deferredInvokes;
    std::recursive_mutex m_deferredInvokesMutex;

    std::mutex m_subLoopsMutex;
};

template<class T, class K = Size>
class Component : public ComponentBase
{
public:
    using ChildType = T;
    using KeyType = K;

    Component(ComponentBase& parent, String id = "Component")
    : ComponentBase(parent, id) {}

    Component(String id = "Component")
    : ComponentBase(id) {}

    virtual ~Component() = default;

// FIXME:
// protected:
    virtual void forEachChild(std::function<void(ChildType&)>&&) = 0;

    virtual bool shouldFireEventForChild(ChildType const&, Event const&) const { return true; }

    template<class U, class Callback>
    void forEachChildTyped(Callback&& function)
    {
        forEachChild([&](auto& child)->void {
            function(static_cast<U&>(child));
        });
    }

    template<class U, class Callback>
    void forEachChildOfType(Callback&& function)
    {
        forEachChild([&](auto& child)->void {
            // TODO: Avoid this dynamic_cast somehow
            // maybe just compare typeinfos
            auto childCasted = dynamic_cast<U*>(&child);
            if(childCasted)
                function(*childCasted);
        });
    }

    virtual void onUpdate() final override
    {
        ComponentBase::onUpdate();
        forEachChild([](auto& child)->void {
            child.onUpdate();
        });
    }

    virtual EventResult fireEvent(Event& event) override
    {
        bool failure = false;
        forEachChild([&](auto& child)->void {
            if(!shouldFireEventForChild(child, event))
                return;
            failure |= (child.fireEvent(event) == EventResult::Failure);
        });
        if(!event.isCanceled())
        {
            failure |= (ComponentBase::fireEvent(event) == EventResult::Failure);
        }
        return EventResult(failure);
    }

protected:
    virtual void onExitInternal() final override
    {
        forEachChild([&](auto& child)->void {
            child.exit(getExitCode());
        });
    }

    virtual EventResult onFinishInternal(int exitCode) final override
    {
        bool failure = (ComponentBase::onFinish(exitCode) == EventResult::Failure);
        forEachChild([&](auto& child)->void {
            failure |= (child.onFinish(exitCode) == EventResult::Failure);
        });
        return EventResult(failure);
    }
};

}
