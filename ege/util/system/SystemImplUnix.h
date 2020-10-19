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

class SystemImplUnix final : public Internal::SystemImpl
{
public:
    // Time
    time_t unixTime();
    System::ExactTime exactTime();

    // FileSystem
    System::FileInfo stat(std::string path);
    std::string getWorkingDirectory();
    bool setWorkingDirectory(std::string dir);
    std::string readLink(std::string link);

    virtual std::string className() { return "EGE::Unix::SystemImplUnix"; }
};

} // Unix

}
