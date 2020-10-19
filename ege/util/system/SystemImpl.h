/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#pragma once

#include "Time.h"

#include <ege/main/Config.h>
#include <string>

namespace EGE
{

namespace Internal
{

class SystemImpl
{
public:
    // Time
    virtual time_t unixTime() { CRASH(); }
    virtual System::ExactTime exactTime() { CRASH(); }

    virtual std::string className() { return "EGE::Internal::SystemImpl"; }
};

} // Internal

namespace System
{

Internal::SystemImpl& impl();

} // System

}
