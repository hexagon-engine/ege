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
#include <ege/util/Time.h>
#include <functional>
#include <string>

#define TIMER_DEBUG 0

namespace EGE
{

class EventLoop;

class Timer
{
public:
    enum class Mode
    {
        Limited,
        Infinite
    };
    EGE_ENUM_YES_NO(Finished);

    Timer(EventLoop* loop, Mode mode, Time interval)
    : m_interval(interval)
    , m_mode(mode)
    , m_loop(loop) {}

    virtual void start();
    virtual void stop();

    // returns true if timer expired and can be removed
    virtual Finished update();

    Timer& setCallback(std::function<void(std::string,Timer*)> func)
    {
        m_callback = func;
        return *this;
    }
    Timer& setUpdateCallback(std::function<void(std::string,Timer*)> func)
    {
        m_updateCallback = func;
        return *this;
    }
    Timer& setName(std::string name)
    {
        m_name = name;
        return *this;
    }
    std::string getName()
    {
        return m_name;
    }
    Timer& setRemainingIterationCount(size_t s)
    {
        m_remainingIterations = s;
        return *this;
    }
    size_t getIterationCount()
    {
        return m_iterations;
    }
    bool isStarted()
    {
        return m_started;
    }
    Time getInterval()
    {
        return m_interval;
    }
    Time getElapsedTime();
    EventLoop* getLoop()
    {
        return m_loop;
    }
    auto getCallback()
    {
        return m_callback;
    }
    auto getUpdateCallback()
    {
        return m_updateCallback;
    }

    void restart();

protected:
    bool m_started = false;
    double m_startTime = 0.f;
    Time m_interval;
    Mode m_mode;
    EventLoop* m_loop;
    std::function<void(std::string,Timer*)> m_callback = nullptr;
    std::function<void(std::string,Timer*)> m_updateCallback = nullptr;
    std::string m_name;
    size_t m_iterations = 0;
    size_t m_remainingIterations = 1;
};

}
