/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#include "ThreadSafeEventLoop.h"

namespace EGE
{

void ThreadSafeEventLoop::addEventHandler(Event::EventType type, std::shared_ptr<EventHandler> handler)
{
    sf::Lock lock(m_eventHandlerMutex);
    EventLoop::addEventHandler(type, handler);
}

void ThreadSafeEventLoop::removeEventHandler(EventHandler* handler)
{
    sf::Lock lock(m_eventHandlerMutex);
    EventLoop::removeEventHandler(handler);
}

EventResult ThreadSafeEventLoop::fireEvent(Event& event)
{
    sf::Lock lock(m_eventHandlerMutex);
    return EventLoop::fireEvent(event);
}

void ThreadSafeEventLoop::addTimer(const std::string& name, std::shared_ptr<Timer> timer, EventLoop::TimerImmediateStart immediateStart)
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

void ThreadSafeEventLoop::addAsyncTask(std::shared_ptr<AsyncTask> task, std::string name)
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

// TODO
void ThreadSafeEventLoop::safeRemoveAsyncTasks()
{
    sf::Lock lock(m_asyncTaskMutex);
    AsyncHandler::safeRemoveAsyncTasks();
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
