/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#pragma once

#include <ege/main/Config.h>
#include <functional>
#include <SFML/System.hpp>

#define ASYNC_TASK_DEBUG 1

namespace EGE
{

class AsyncLoop;

class AsyncTask
{
public:
    struct State
    {
        int returnCode;
        bool finished;
    };

    // WORKER is running in OTHER thread
    // CALLBACK is running in MAIN thread (The same as AsyncLoop)
    // So if you want callback to be called, you must update AsyncLoop!
    AsyncTask(std::function<int()> worker, std::function<void(State)> callback);

    // NOTE: it calls terminate()! so it's recommended to call stop() before
    // destroying AsyncTasks.
    virtual ~AsyncTask();

    void start();
    State update();

    // Safe.
    virtual void stop() {}

    // Not safe!
    void terminate();

    void setName(std::string name)
    {
        if(m_name.empty())
            m_name = name;
    }

    std::string getName()
    {
        return m_name;
    }

    bool finished()
    {
        return m_currentState.finished;
    }

private:
    void entryPoint();

    sf::Thread m_thread;
    std::function<int()> m_worker;
    std::function<void(State)> m_callback;

    State m_currentState {0, false};
    std::string m_name;
};

}
