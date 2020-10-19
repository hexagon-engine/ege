/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#pragma once

#include <ctime>

namespace EGE
{

namespace System
{

struct ExactTime
{
    time_t s;
    long long ns;
};

// seconds since 1970-01-01
time_t unixTime();

ExactTime exactTime();

}

}
