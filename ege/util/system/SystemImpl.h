/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#pragma once

#include "FileSystem.h"
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

    // FileSystem
    virtual System::FileInfo stat(std::string) { CRASH(); }
    virtual std::string getWorkingDirectory() { CRASH(); }
    virtual bool setWorkingDirectory(std::string) { CRASH(); }
    virtual std::string readLink(std::string) { CRASH(); }
    virtual bool testFileAccess(std::string, System::FileOpenModeMask) { CRASH(); }

    // Global
    virtual std::string getErrorMessage() { CRASH(); }

    virtual std::string className() { return "EGE::Internal::SystemImpl"; }
};

} // Internal

namespace System
{

Internal::SystemImpl& impl();

} // System

}
