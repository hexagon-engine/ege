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

#include <ege/main/Config.h>
#include <ege/util/PointerUtils.h>
#include <functional>
#include <SFML/System.hpp>

#define ASYNC_TASK_DEBUG 0

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

    // WORKER runs in OTHER thread
    // CALLBACK runs in MAIN thread (The same as AsyncLoop)
    // So if you want callback to be called, you must update AsyncLoop!
    AsyncTask(std::function<int()> worker, std::function<void(State)> callback);

    // NOTE: it calls terminate()! so it's recommended to call stop() before
    // destroying AsyncTasks.
    virtual ~AsyncTask();

    void start();
    State update();

    // Safe.
    virtual void stop() {}
    void wait();

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
