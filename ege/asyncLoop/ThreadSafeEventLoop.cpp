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

#include "ThreadSafeEventLoop.h"

namespace EGE
{

void ThreadSafeEventLoop::addTimer(const std::string& name, SharedPtr<Timer> timer, EventLoop::TimerImmediateStart immediateStart)
{
    sf::Lock lock(m_timerMutex);
    EventLoop::addTimer(name, timer, immediateStart);
}

std::vector<std::weak_ptr<Timer>> ThreadSafeEventLoop::getTimers(const std::string& timer)
{
    sf::Lock lock(m_timerMutex);
    return EventLoop::getTimers(timer);
}
void ThreadSafeEventLoop::removeTimer(const std::string& timer)
{
    sf::Lock lock(m_timerMutex);
    EventLoop::removeTimer(timer);
}

void ThreadSafeEventLoop::onUpdate()
{
    EventLoop::onUpdate();
    AsyncHandler::updateAsyncTasks();
}

void ThreadSafeEventLoop::deferredInvoke(std::function<void()> func)
{
    sf::Lock lock(m_timerMutex);
    EventLoop::deferredInvoke(func);
}

void ThreadSafeEventLoop::addAsyncTask(SharedPtr<AsyncTask> task, std::string name)
{
    sf::Lock lock(m_asyncTaskMutex);
    AsyncHandler::addAsyncTask(task, name);
}

// unsafe due to thread termination
void ThreadSafeEventLoop::removeAsyncTasks(std::string name)
{
    sf::Lock lock(m_asyncTaskMutex);
    AsyncHandler::removeAsyncTasks(name);
}

std::vector<std::weak_ptr<AsyncTask>> ThreadSafeEventLoop::getAsyncTasks(std::string name)
{
    sf::Lock lock(m_asyncTaskMutex);
    return AsyncHandler::getAsyncTasks(name);
}

void ThreadSafeEventLoop::updateTimers()
{
    sf::Lock lock(m_timerMutex);
    EventLoop::updateTimers();
}

void ThreadSafeEventLoop::updateAsyncTasks()
{
    sf::Lock lock(m_asyncTaskMutex);
    AsyncHandler::updateAsyncTasks();
}

}
