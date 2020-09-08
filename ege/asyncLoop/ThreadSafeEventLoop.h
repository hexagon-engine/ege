/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
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
