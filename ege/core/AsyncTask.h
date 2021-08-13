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
#include <thread>
#include <atomic>

#define ASYNC_TASK_DEBUG 0

namespace EGE
{

class AsyncTask
{
public:
    struct State
    {
        int returnCode;
        bool finished;
    };

    inline static constexpr State UnfinishedState = { 0, false };

    // WORKER runs in OTHER thread
    // CALLBACK runs in MAIN thread (The same as Component)
    // So if you want callback to be called, you must update Component!
    AsyncTask(std::function<int(AsyncTask& task)> worker, std::function<void(State)> callback);

    virtual ~AsyncTask();

    void start();
    State update();

    void requestStop() { m_stopRequested.store(true); }
    void wait();

    // Wait for thread but assume it will finish soon and call
    // callback (with unfinished state) before it returns.
    void terminate();

    bool stopRequested() const { return m_stopRequested; }

    void setName(std::string name)
    {
        if(m_name.empty())
            m_name = name;
    }

    std::string getName() const { return m_name; }
    bool finished() const { return m_finished.load(); }

private:
    void entryPoint();

    std::thread m_thread;
    std::function<int(AsyncTask& task)> m_worker;
    std::function<void(State)> m_callback;

    std::atomic<int> m_returnCode { 0 };
    std::atomic<bool> m_finished { false };
    std::atomic<bool> m_stopRequested { false };
    std::string m_name;
};

}
