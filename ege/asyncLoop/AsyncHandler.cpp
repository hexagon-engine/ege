/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#include "AsyncHandler.h"

#include <ege/debug/Logger.h>

namespace EGE
{

void AsyncHandler::addAsyncTask(std::shared_ptr<AsyncTask> task, std::string name)
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
