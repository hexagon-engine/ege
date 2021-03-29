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

#include "AsyncHandler.h"

#include <ege/debug/Logger.h>

namespace EGE
{

void AsyncHandler::addAsyncTask(SharedPtr<AsyncTask> task, std::string name)
{
    task->setName(name);
    task->start();
    m_asyncTasks.insert(std::make_pair(name, task));
}

std::vector<std::weak_ptr<AsyncTask>> AsyncHandler::getAsyncTasks(std::string name)
{
    std::vector<std::weak_ptr<AsyncTask>> tasks;
    decltype(m_asyncTasks)::iterator it;
    while((it = m_asyncTasks.find(name)) != m_asyncTasks.end())
    {
        tasks.push_back(it->second);
    }
    return tasks;
}

// unsafe due to thread termination
void AsyncHandler::removeAsyncTasks(std::string name)
{
    decltype(m_asyncTasks)::iterator it;
    while((it = m_asyncTasks.find(name)) != m_asyncTasks.end())
    {
        m_asyncTasks.erase(it);
    }
}

void AsyncHandler::safeRemoveAsyncTasks()
{
    ASSERT(false);
}

void AsyncHandler::updateAsyncTasks()
{
    for(auto it = m_asyncTasks.begin(); it != m_asyncTasks.end(); it++)
    {
        auto task = *it;
        AsyncTask::State state = task.second.get()->update();
        if(state.finished)
        {
            if(state.returnCode != 0)
                err(LogLevel::Verbose) << "001C EGE/asyncLoop: AsyncTask[" << task.first << "] worker finished with non-zero (" << state.returnCode << ") status!";

            m_asyncTasks.erase(it);
            if(m_asyncTasks.empty())
                return;

            it = m_asyncTasks.find(task.first);
        }
    }
}

}
