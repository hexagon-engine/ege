/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#pragma once

#include "AsyncTask.h"

#include <ege/loop/EventLoop.h>

#include <map>
#include <memory>
#include <string>
#include <vector>

namespace EGE
{

class AsyncHandler
{
public:
    virtual void addAsyncTask(std::shared_ptr<AsyncTask> task, std::string name = "");

    // unsafe due to thread termination
    virtual void removeAsyncTasks(std::string name = "");

    virtual std::vector<std::weak_ptr<AsyncTask>> getAsyncTasks(std::string name = "");

    // TODO
    virtual void safeRemoveAsyncTasks();

protected:
    virtual void updateAsyncTasks();

private:
    std::multimap<std::string, std::shared_ptr<AsyncTask>> m_asyncTasks;

};

}
