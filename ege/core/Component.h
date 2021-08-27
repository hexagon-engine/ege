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
#include "EventResult.h"
#include "EventTarget.h"
#include "Behaviour.h"
#include "Timer.h"

#include <ege/debug/InspectorNode.h>
#include <ege/debug/Logger.h>
#include <ege/debug/Profiler.h>
#include <ege/debug/ProfilerSectionStarter.h>

#include <map>
#include <memory>
#include <mutex>
#include <queue>
#include <sstream>
#include <thread>
#include <vector>

namespace EGE
{

// TODO: Merge this with InspectorNode
class ComponentBase : public InspectorNode, public EventTarget
{
public:
    explicit ComponentBase(ComponentBase& parent, String id = "ComponentBase")
    : InspectorNode(&parent, id) {}

    explicit ComponentBase(String id = "ComponentBase")
    : InspectorNode(id) {}

    EGE_ENUM_YES_NO(TimerImmediateStart);

    void addTimer(std::string const& name, SharedPtr<Timer> timer, TimerImmediateStart start = TimerImmediateStart::Yes);
    std::vector<std::weak_ptr<Timer>> getTimers(const std::string& timer);
    void removeTimer(std::string const& timer);
    void deferredInvoke(std::function<void()> func);

    template<class T, class... Args>
    T& addNewBehaviour(Args&&... args)
    {
        ASSERT(instanceof(this, typename T::ComponentType));
        auto behaviour = makeUnique<T>(static_cast<typename T::ComponentType&>(*this), std::forward<Args>(args)...);
        auto behaviourRawPtr = behaviour.get();
        addBehaviour(std::move(behaviour));
        return *behaviourRawPtr;
    }

    // T must be derived from Internal::_BehaviourBase
    // Callback must be a function of type void(T&)
    template<class T, class Callback>
    void forEachBehaviourOfType(Callback&& callback)
    {
        ProfilerSectionStarter starter(*getProfiler(), "lock");
        std::lock_guard<std::mutex> lock(m_behaviourMutex);

        for(auto& behaviour: m_behaviours)
        {
            starter.switchSection("Behaviour<" + std::to_string(behaviour->typeId().id()) + ">/callback");
            if(behaviour->typeId() == T::typeIdStatic())
                callback(static_cast<T&>(*behaviour));
        }
    }

    // get in-loop time in ticks or ms
    // it should be used ONLY for comparisions
    double time(Time::Unit unit);

    virtual void onUpdate();

    virtual void exit(int exitCode = 0);

    void addAsyncTask(SharedPtr<AsyncTask> task, std::string name = "");
    void removeAsyncTasks(std::string name = "");
    std::vector<std::weak_ptr<AsyncTask>> getAsyncTasks(std::string name = "");

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
    void addBehaviour(UniquePtr<Internal::_BehaviourBase>);
    virtual void onProfilerResults(Profiler const&) {}

    virtual void onExitInternal() {}
    virtual EventResult onFinishInternal(int exitCode) { return onFinish(exitCode); }

    virtual EventResult fireEventOnBehaviours(Event&);

private:
    std::atomic<TickCount> m_ticks = 0;
    std::atomic<int> m_exitCode = 0;
    std::atomic<bool> m_running = true;

    virtual void updateTimers();
    virtual void updateAsyncTasks();
    virtual void callDeferredInvokes();
    virtual void updateBehaviours();

    std::multimap<std::string, SharedPtr<AsyncTask>> m_asyncTasks;
    std::mutex m_asyncTasksMutex;

    std::multimap<std::string, SharedPtr<Timer>> m_timers;
    std::mutex m_timersMutex;

    std::queue<std::function<void()>> m_deferredInvokes;
    std::recursive_mutex m_deferredInvokesMutex;

    Vector<UniquePtr<Internal::_BehaviourBase>> m_behaviours;
    std::mutex m_behaviourMutex;
};

template<class T, class K = Size>
class Component : public ComponentBase
{
public:
    using ChildType = T;
    using KeyType = K;

    explicit Component(ComponentBase& parent, String id = "Component")
    : ComponentBase(parent, id) {}

    explicit Component(String id = "Component")
    : ComponentBase(id) {}

    virtual ~Component() = default;

// FIXME:
// protected:
    template<class Callback>
    void forEachChild(Callback&& function)
    {
        _ForEachChildCallback<Callback> callback(std::move(function));
        forEachChildImpl(callback);
    }

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
        if(!getProfiler())
        {
            createProfiler();
            m_profilerCreated = true;
        }

        ComponentBase::onUpdate();
        forEachChild([this](auto& child)->void {
            std::ostringstream oss;
            oss << "Component<" << child.isnName() << "@" << std::hex << &child << ">/onUpdate";
            ProfilerSectionStarter starter(*getProfiler(), oss.str());
            child.onUpdate();
        });

        if(m_profilerCreated)
        {
            destroyProfilerIfNeeded();
            m_profilerCreated = false;
        }
    }

    virtual EventResult fireEvent(Event& event) override
    {
        bool failure = false;
        forEachChild([&](auto& child)->void {
            if(!shouldFireEventForChild(child, event))
                return;
            failure |= (child.fireEvent(event) == EventResult::Failure);
        });
        failure |= (fireEventOnBehaviours(event) == EventResult::Failure);
        if(!event.isCanceled())
        {
            failure |= (ComponentBase::fireEvent(event) == EventResult::Failure);
        }
        return EventResult(failure);
    }

protected:
    class _ForEachChildCallbackBase
    {
    public:
        virtual void operator()(ChildType&) = 0;
    };

    template<class U>
    class _ForEachChildCallback : public _ForEachChildCallbackBase
    {
    public:
        _ForEachChildCallback(U&& callback)
        : m_callback(std::move(callback)) {}

        virtual void operator()(ChildType& child) override
        {
            m_callback(child);
        }

    private:
        U m_callback;
    };

    virtual void forEachChildImpl(_ForEachChildCallbackBase&) = 0;

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

    virtual bool shouldFireEventForChild(ChildType const&, Event const&) const { return true; }

private:
    bool m_profilerCreated = false;
};

}
