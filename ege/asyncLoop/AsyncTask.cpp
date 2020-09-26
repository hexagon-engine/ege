/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#include "AsyncTask.h"

#include <ege/main/Config.h>
#include <ege/util/PointerUtils.h>

namespace EGE
{

AsyncTask::AsyncTask(std::function<int()> worker, std::function<void(AsyncTask::State)> callback)
: m_thread(&AsyncTask::entryPoint, this), m_worker(worker), m_callback(callback) {}

AsyncTask::~AsyncTask()
{
    if(!m_currentState.finished)
        m_thread.terminate();
}

void AsyncTask::start()
{
    m_thread.launch();
}

AsyncTask::State AsyncTask::update()
{
    if(m_currentState.finished)
    {
        DBG(ASYNC_TASK_DEBUG, "worker finished with status " + std::to_string(m_currentState.returnCode));
        m_callback(m_currentState);
    }
    return m_currentState;
}

void AsyncTask::entryPoint()
{
    // atomic?
    m_currentState = AsyncTask::State{m_worker(), true};
}

void AsyncTask::wait()
{
    m_thread.wait();
}

void AsyncTask::terminate()
{
    // it calls callback with m_currentState.finished == false
    if(m_callback)
        m_callback(m_currentState);

    m_thread.terminate();
}

}
