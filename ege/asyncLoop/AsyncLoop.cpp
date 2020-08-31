/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#include "AsyncLoop.h"

namespace EGE
{

void AsyncLoop::onUpdate()
{
    EventLoop::onUpdate();
    AsyncHandler::updateAsyncTasks();
}

}
