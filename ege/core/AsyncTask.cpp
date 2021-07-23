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

#include "AsyncTask.h"

#include <ege/main/Config.h>
#include <ege/util/PointerUtils.h>

namespace EGE
{

AsyncTask::AsyncTask(std::function<int(AsyncTask& task)> worker, std::function<void(AsyncTask::State)> callback)
: m_worker(std::move(worker)), m_callback(std::move(callback)) {}

AsyncTask::~AsyncTask()
{
    requestStop();
    wait();
}

void AsyncTask::start()
{
    m_thread = std::thread(&AsyncTask::entryPoint, this);
}

AsyncTask::State AsyncTask::update()
{
    bool finished = m_finished.load();
    if(finished)
    {
        int returnCode = m_returnCode.load();
        DBG(ASYNC_TASK_DEBUG, "worker finished with status " + std::to_string(returnCode));
        auto state = State{returnCode, true};
        ASSERT(m_callback);
        m_callback(state);
        return state;
    }
    return UnfinishedState;
}

void AsyncTask::entryPoint()
{
    m_returnCode.store(m_worker(*this));
    m_finished.store(true);
}

void AsyncTask::wait()
{
    if(m_thread.joinable())
        m_thread.join();
}

void AsyncTask::terminate()
{
    if(m_callback)
        m_callback(UnfinishedState);

    requestStop();
    wait();
}

}
