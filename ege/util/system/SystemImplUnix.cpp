/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#include "SystemImplUnix.h"

#include <memory>

namespace EGE
{

namespace System
{

std::unique_ptr<Internal::SystemImpl> g_impl = std::make_unique<Unix::SystemImplUnix>();

} // System

namespace Unix
{

time_t SystemImplUnix::unixTime()
{
    return time(nullptr);
}

System::ExactTime SystemImplUnix::exactTime()
{
    timespec _ts;
    if(clock_gettime(CLOCK_REALTIME, &_ts) < 0)
        return {};

    return {_ts.tv_sec, _ts.tv_nsec};
}

} // Unix

}
