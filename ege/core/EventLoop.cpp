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

#include "EventLoop.h"

#include "TimerEvent.h"

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

void EventLoop::addTimer(const std::string& name, SharedPtr<Timer> timer, EventLoop::TimerImmediateStart immediateStart)
{
    ASSERT(timer);
    timer->setName(name);
    if(immediateStart == EventLoop::TimerImmediateStart::Yes)
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
            this->onTimerTick(_timer);
       });
    }
    std::lock_guard<std::mutex> lock(m_timersMutex);
    m_timers.insert(std::make_pair(name, timer));
}

std::vector<std::weak_ptr<Timer>> EventLoop::getTimers(const std::string& timer)
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
void EventLoop::removeTimer(const std::string& timer)
{
    decltype(m_timers)::iterator it;
    std::lock_guard<std::mutex> lock(m_timersMutex);
    while((it = m_timers.find(timer)) != m_timers.end())
    {
        m_timers.erase(it);
    }
}

void EventLoop::onUpdate()
{
    if(m_profiler) m_profiler->startSection("onTick");
    onTick(m_ticks.fetch_add(1) + 1);
    if(m_profiler) m_profiler->endStartSection("updateSubloops");
    updateSubloops();
    if(m_profiler) m_profiler->endStartSection("updateTimers");
    updateTimers();
    if(m_profiler) m_profiler->endStartSection("callDeferredInvokes");
    callDeferredInvokes();
    if(m_profiler) m_profiler->endStartSection("updateAsyncTasks");
    updateAsyncTasks();
    if(m_profiler) m_profiler->endSection();
}

void EventLoop::updateSubloops()
{
    // Update all subloops
    {
        std::lock_guard<std::mutex> lock(m_subLoopsMutex);
        for(auto& subLoop: m_subLoops)
            subLoop->onUpdate();
    }

    // Remove subloops that exited.
    {
        std::lock_guard<std::mutex> lock(m_subLoopsMutex);
        auto it = std::remove_if(m_subLoops.begin(), m_subLoops.end(), [&](SharedPtr<EventLoop> otherLoop) {
            return !otherLoop->isRunning();
        });

        if(it != m_subLoops.end())
        {
            ege_log.verbose() << "EventLoop: Cleaning up exited subloops";
            it->get()->onFinish(it->get()->m_exitCode.load());
            m_subLoops.erase(it);
        }
    }
}

void EventLoop::updateTimers()
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

            onTimerFinish(timer.second.get());
            m_timers.erase(it);

            if(m_timers.empty())
                return;

            it = m_timers.find(timer.first);
        }
    }
}

void EventLoop::deferredInvoke(std::function<void()> func)
{
    std::lock_guard<std::mutex> lock(m_deferredInvokesMutex);
    m_deferredInvokes.push(func);
}

void EventLoop::callDeferredInvokes()
{
    std::lock_guard<std::mutex> lock(m_deferredInvokesMutex);
    while(!m_deferredInvokes.empty())
    {
        m_deferredInvokes.front()();
        m_deferredInvokes.pop();
    }
}

double EventLoop::time(Time::Unit unit)
{
    if(unit == Time::Unit::Ticks)
        return m_ticks.load();
    else if(unit == Time::Unit::Seconds)
        return System::exactTime().seconds();
    CRASH();
}

void EventLoop::exit(int exitCode)
{
    m_exitCode.store(exitCode);
    m_running.store(false);

    // Deferred invoke to prevent deadlocks when exiting main loop from subloop event handler
    deferredInvoke([&] {
        std::lock_guard<std::mutex> lock(m_subLoopsMutex);
        for(auto& subLoop: m_subLoops)
        {
            subLoop->exit(exitCode);
        }
    });

    onExit(exitCode);
}

bool EventLoop::addSubLoop(SharedPtr<EventLoop> loop)
{
    ASSERT(loop);
    loop->isnSetParent(this);

    if(loop->onLoad() == EventResult::Failure)
        return false;

    {
        std::lock_guard<std::mutex> lock(m_subLoopsMutex);
        m_subLoops.push_back(loop);
    }

    return true;
}

void EventLoop::removeSubLoop(EventLoop& loop)
{
    loop.exit(0);
}

void EventLoop::addAsyncTask(SharedPtr<AsyncTask> task, std::string name)
{
    task->setName(name);
    task->start();
    std::lock_guard<std::mutex> lock(m_asyncTasksMutex);
    m_asyncTasks.insert(std::make_pair(name, task));
}

std::vector<std::weak_ptr<AsyncTask>> EventLoop::getAsyncTasks(std::string name)
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

void EventLoop::removeAsyncTasks(std::string name)
{
    decltype(m_asyncTasks)::iterator it;
    std::lock_guard<std::mutex> lock(m_asyncTasksMutex);
    while((it = m_asyncTasks.find(name)) != m_asyncTasks.end())
    {
        m_asyncTasks.erase(it);
    }
}

void EventLoop::updateAsyncTasks()
{
    std::lock_guard<std::mutex> lock(m_asyncTasksMutex);
    for(auto it = m_asyncTasks.begin(); it != m_asyncTasks.end(); it++)
    {
        auto task = *it;
        AsyncTask::State state = task.second.get()->update();
        if(state.finished)
        {
            if(state.returnCode != 0)
                ege_log.error() << "EventLoop: AsyncTask[" << task.first << "] worker finished with non-zero (" << state.returnCode << ") status!";

            m_asyncTasks.erase(it);
            if(m_asyncTasks.empty())
                return;

            it = m_asyncTasks.find(task.first);
        }
    }
}

}
