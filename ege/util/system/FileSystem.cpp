/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#include "FileSystem.h"

#include "SystemImpl.h"

namespace EGE
{

namespace System
{

FileInfo stat(std::string path)
{
    return impl().stat(path);
}

std::string getWorkingDirectory()
{
    return impl().getWorkingDirectory();
}

bool setWorkingDirectory(std::string dir)
{
    return impl().setWorkingDirectory(dir);
}

std::string readLink(std::string link)
{
    return impl().readLink(link);
}

} // System

}
