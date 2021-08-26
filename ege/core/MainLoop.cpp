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

#include "MainLoop.h"

#include <ege/debug/ProfilerSectionStarter.h>

namespace EGE
{

int MainLoop::run()
{
    auto result = onLoad();

    if(result == EventResult::Failure)
    {
        ege_log.critical() << "MainLoop: onLoad() failed";
        return 0x0001;
    }

    Clock tickClock(*this);
    while(isRunning())
    {
        createProfiler();
        auto profiler = getProfiler();
        profiler->start();
        tickClock.restart();
        {
            ProfilerSectionStarter starter(*profiler, "MainLoop/update");
            onUpdate();

            // Limit tick time / frame rate
            starter.switchSection("MainLoop/tickLimit");
            if(m_minTickTime.getValue() > 0.0 && m_minTickTime.getValue() > tickClock.getElapsedTime())
            {
                EGE::System::sleep(EGE::System::ExactTime::fromSeconds(m_minTickTime.getValue() - tickClock.getElapsedTime()));
            }
        }
        profiler->end();
        onProfilerResults(*profiler);
        if(m_requestedProfilerDisplay)
            ege_log.info() << profiler->toString();
        destroyProfiler();
    }

    int exitCode = getExitCode();
    result = onFinishInternal(exitCode);

    if(result == EventResult::Failure)
    {
        ege_log.critical() << "MainLoop: onFinish() failed";
        return 0x0002;
    }

    EGE::Inspector::instance().display(EGE::mainLogger());
    return exitCode;
}

}
