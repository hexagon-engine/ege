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

#include "BasicComponent.h"

#include <ege/core/Clock.h>
#include <ege/debug/Inspector.h>
#include <ege/debug/Logger.h>
#include <ege/util/system.h>

#include <atomic>

namespace EGE
{

class MainLoop : public BasicComponent<ComponentBase>
{
public:
    MainLoop(String id = "MainLoop")
    : BasicComponent<ComponentBase>(id) {}

    int run();

    void setMinimalTickTime(Time time) { m_minTickTime = time; }
    void setMaxTicksPerSecond(int value) { setMinimalTickTime({1.0 / value}); }

    Time getLastTickTime() const  { return m_lastTickTime; }
    double getLastTicksPerSecond() const { return 1.0 / m_lastTickTime.getValue(); }

    void requestProfilerDisplay() { m_requestedProfilerDisplay = true; }

private:
    Time m_minTickTime = {0.0, Time::Unit::Seconds};
    Time m_lastTickTime;
    bool m_requestedProfilerDisplay = false;
};

}
