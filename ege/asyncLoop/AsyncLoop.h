/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#pragma once

#include "AsyncHandler.h"

#include <ege/loop/EventLoop.h>

#include <map>
#include <memory>
#include <string>
#include <vector>

namespace EGE
{

class AsyncLoop : public AsyncHandler, public EventLoop
{
public:
    virtual void onUpdate();
};

}
