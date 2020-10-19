/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#pragma once

#include "SystemImpl.h"

namespace EGE
{

namespace Unix
{

class SystemImplUnix : public Internal::SystemImpl
{
public:
    // Time
    virtual time_t unixTime();
    virtual System::ExactTime exactTime();

    virtual std::string className() { return "EGE::Unix::SystemImplUnix"; }
};

} // Unix

}
