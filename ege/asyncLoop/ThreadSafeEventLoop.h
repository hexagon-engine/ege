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

#include <ege/asyncLoop/AsyncLoop.h>
#include <ege/loop/EventHandler.h>
#include <map>
#include <memory>
#include <SFML/System.hpp>
#include <vector>

namespace EGE
{

class ThreadSafeEventLoop : public AsyncLoop
{
public:
    // from EVENT LOOP
    virtual void addEventHandler(Event::EventType type, std::shared_ptr<EventHandler> handler);

    // NOTE: removing event handlers in event handler is UB
    virtual void removeEventHandler(EventHandler* handler);

    virtual EventResult fireEvent(Event& event);

    virtual void addTimer(const std::string& name, std::shared_ptr<Timer> timer, EventLoop::TimerImmediateStart start = EventLoop::TimerImmediateStart::Yes);
    virtual std::vector<std::weak_ptr<Timer>> getTimers(const std::string& timer);
    virtual void removeTimer(const std::string& timer);
    virtual void onUpdate();
    virtual void deferredInvoke(std::function<void()> func);

    // ASYNC TASKS
    virtual void addAsyncTask(std::shared_ptr<AsyncTask> task, std::string name = "");

    // unsafe due to thread termination
    virtual void removeAsyncTasks(std::string name = "");
    virtual std::vector<std::weak_ptr<AsyncTask>> getAsyncTasks(std::string name = "");
    virtual void safeRemoveAsyncTasks();

protected:
    sf::Mutex m_eventHandlerMutex;
    sf::Mutex m_timerMutex;
    sf::Mutex m_asyncTaskMutex;

    virtual void updateTimers();
    virtual void updateAsyncTasks();
};

}
