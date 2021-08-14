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

#include "Component.h"

#include "TimerEvent.h"

#include <ege/core/Behaviour.h>
#include <ege/core/TickEvent.h>
#include <ege/debug/Inspector.h>
#include <ege/debug/Logger.h>
#include <ege/main/Config.h>
#include <ege/util/PointerUtils.h>
#include <ege/util/system/Time.h>
#include <iostream>
#include <string.h>

#ifdef WIN32
    #include <windows.h>
#endif // WIN32

namespace EGE
{

void ComponentBase::addTimer(const std::string& name, SharedPtr<Timer> timer, TimerImmediateStart immediateStart)
{
    ASSERT(timer);
    timer->setName(name);
    if(immediateStart == TimerImmediateStart::Yes)
    {
        TimerStartEvent event(*timer);
        events<TimerStartEvent>().fire(event);
        if(event.isCanceled())
        {
            return;
        }
        timer->start();
    }
    if(!timer->getCallback())
    {
        timer->setCallback([this](std::string, Timer* _timer) {
            TimerTickEvent event(*_timer);
            events<TimerTickEvent>().fire(event);

            if(event.isCanceled())
                return;
       });
    }
    // deferredInvoke to prevent deadlocks when adding timers from timer callback
    deferredInvoke([this, name, timer] {
        std::lock_guard<std::mutex> lock(m_timersMutex);
        m_timers.insert(std::make_pair(name, timer));
    });
}

std::vector<std::weak_ptr<Timer>> ComponentBase::getTimers(const std::string& timer)
{
    std::vector<std::weak_ptr<Timer>> timers;
    decltype(m_timers)::iterator it;
    std::lock_guard<std::mutex> lock(m_timersMutex);
    while((it = m_timers.find(timer)) != m_timers.end())
    {
        timers.push_back(it->second);
    }
    return timers;
}
void ComponentBase::removeTimer(const std::string& timer)
{
    decltype(m_timers)::iterator it;
    std::lock_guard<std::mutex> lock(m_timersMutex);
    while((it = m_timers.find(timer)) != m_timers.end())
    {
        m_timers.erase(it);
    }
}

void ComponentBase::updateTimers()
{
    std::lock_guard<std::mutex> lock(m_timersMutex);
    for(auto it = m_timers.begin(); it != m_timers.end(); it++)
    {
        auto timer = *it;
        if(timer.second.get()->update() == Timer::Finished::Yes)
        {
            TimerFinishEvent event(*timer.second);
            events<TimerFinishEvent>().fire(event);
            if(event.isCanceled())
                continue;

            m_timers.erase(it);

            if(m_timers.empty())
                return;

            it = m_timers.find(timer.first);
        }
    }
}

void ComponentBase::addBehaviour(UniquePtr<Internal::_BehaviourBase> behaviour)
{
    std::lock_guard<std::mutex> lock(m_behaviourMutex);
    m_behaviours.push_back(std::move(behaviour));
}

void ComponentBase::updateBehaviours()
{
    std::lock_guard<std::mutex> lock(m_behaviourMutex);
    for(auto& behaviour: m_behaviours)
        behaviour->onUpdate();
}

void ComponentBase::deferredInvoke(std::function<void()> func)
{
    std::lock_guard<std::recursive_mutex> lock(m_deferredInvokesMutex);
    m_deferredInvokes.push(func);
}

void ComponentBase::callDeferredInvokes()
{
    std::lock_guard<std::recursive_mutex> lock(m_deferredInvokesMutex);
    while(!m_deferredInvokes.empty())
    {
        m_deferredInvokes.front()();
        m_deferredInvokes.pop();
    }
}

double ComponentBase::time(Time::Unit unit)
{
    if(unit == Time::Unit::Ticks)
        return m_ticks.load();
    else if(unit == Time::Unit::Seconds)
        return System::exactTime().seconds();
    CRASH();
}

void ComponentBase::exit(int exitCode)
{
    m_exitCode.store(exitCode);
    m_running.store(false);
}

void ComponentBase::addAsyncTask(SharedPtr<AsyncTask> task, std::string name)
{
    task->setName(name);
    task->start();
    std::lock_guard<std::mutex> lock(m_asyncTasksMutex);
    m_asyncTasks.insert(std::make_pair(name, task));
}

std::vector<std::weak_ptr<AsyncTask>> ComponentBase::getAsyncTasks(std::string name)
{
    std::vector<std::weak_ptr<AsyncTask>> tasks;
    decltype(m_asyncTasks)::iterator it;
    {
        std::lock_guard<std::mutex> lock(m_asyncTasksMutex);
        while((it = m_asyncTasks.find(name)) != m_asyncTasks.end())
        {
            tasks.push_back(it->second);
        }
    }
    return tasks;
}

void ComponentBase::removeAsyncTasks(std::string name)
{
    decltype(m_asyncTasks)::iterator it;
    std::lock_guard<std::mutex> lock(m_asyncTasksMutex);
    while((it = m_asyncTasks.find(name)) != m_asyncTasks.end())
    {
        m_asyncTasks.erase(it);
    }
}

void ComponentBase::updateAsyncTasks()
{
    std::lock_guard<std::mutex> lock(m_asyncTasksMutex);
    for(auto it = m_asyncTasks.begin(); it != m_asyncTasks.end(); it++)
    {
        auto task = *it;
        AsyncTask::State state = task.second.get()->update();
        if(state.finished)
        {
            if(state.returnCode != 0)
                ege_log.error() << "Component: AsyncTask[" << task.first << "] worker finished with non-zero (" << state.returnCode << ") status!";

            m_asyncTasks.erase(it);
            if(m_asyncTasks.empty())
                return;

            it = m_asyncTasks.find(task.first);
        }
    }
}

void ComponentBase::onUpdate()
{
    auto profiler = getProfiler();

    ProfilerSectionStarter starter(*profiler, "onTick");
    fire<TickEvent>(m_ticks.fetch_add(1));
    onTick();

    starter.switchSection("updateBehaviours");
    updateBehaviours();

    starter.switchSection("updateTimers");
    updateTimers();

    starter.switchSection("callDeferredInvokes");
    callDeferredInvokes();

    starter.switchSection("updateAsyncTasks");
    updateAsyncTasks();
}

}
