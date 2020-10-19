/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#include "Time.h"

#include "SystemImpl.h"

namespace EGE
{

namespace System
{

time_t unixTime()
{
    return impl().unixTime();
}

ExactTime exactTime()
{
    return impl().exactTime();
}

} // System

}
