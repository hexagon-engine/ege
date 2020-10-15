/*
*
*   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*
*      ,----  ,----  ,----
*      |      |      |
*      |----  | --,  |----
*      |      |   |  |
*      '----  '---'  '----
*
*     Framework Library for Hexagon
*
*    Copyright (c) Sppmacd 2020 - 2021
*
*    Permission is hereby granted, free of charge, to any person obtaining a copy
*    of this software and associated documentation files (the "Software"), to deal
*    in the Software without restriction, including without limitation the rights
*    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
*    copies of the Software, and to permit persons to whom the Software is
*    furnished to do so, subject to the following conditions:
*
*    The above copyright notice and this permission notice shall be included in all
*    copies or substantial portions of the Software.
*
*    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
*    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
*    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
*    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
*    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
*    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
*    SOFTWARE.
*
*   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*
*/

#include "SystemImplWin32.h"

#include <ege/util/Types.h>

#include <dirent.h>
#include <errno.h>
#include <limits.h>
#include <memory>
#include <string.h>
#include <sys/stat.h>
#include <windows.h>

namespace EGE
{

namespace System
{

std::unique_ptr<Internal::SystemImpl> g_impl = std::make_unique<Windows::SystemImplWin32>();

} // System

namespace Windows
{

// Time
time_t SystemImplWin32::unixTime()
{
    return time(nullptr);
}

System::ExactTime SystemImplWin32::exactTime()
{
    LARGE_INTEGER frequency; // ticks per second
    QueryPerformanceFrequency(&frequency); 
    LARGE_INTEGER performanceCount; // number of ticks
    QueryPerformanceCounter(&performanceCount);
    Uint64 nanoseconds = performanceCount.QuadPart * 1'000'000'000 / frequency.QuadPart;
    return System::ExactTime::fromNanoseconds(nanoseconds);
}

void SystemImplWin32::sleep(System::ExactTime)
{
    NOT_IMPLEMENTED();
}

// FileSystem
System::FileInfo SystemImplWin32::stat(std::string /*path*/)
{
    NOT_IMPLEMENTED();
}

std::string SystemImplWin32::getWorkingDirectory()
{
    auto requiredSize = GetCurrentDirectory(0, nullptr);
    if(!requiredSize)
        return "";
    auto buffer = new char[requiredSize];
    GetCurrentDirectory(requiredSize, buffer);
    std::string str(buffer, requiredSize - 1);
    delete buffer;
    return str;
}

bool SystemImplWin32::setWorkingDirectory(std::string dir)
{
    // TODO: Error handling
    return SetCurrentDirectory(dir.c_str());
}

std::string SystemImplWin32::readLink(std::string /*link*/)
{
    NOT_IMPLEMENTED();
}

bool SystemImplWin32::testFileAccess(std::string /*path*/, System::FileOpenModeMask /*mode*/)
{
    NOT_IMPLEMENTED();
}

bool SystemImplWin32::createDirectory(std::string /*path*/, System::FileMode /*mode*/)
{
    NOT_IMPLEMENTED();
}

bool SystemImplWin32::removeFile(std::string /*path*/)
{
    NOT_IMPLEMENTED();
}

std::vector<std::string> SystemImplWin32::listFiles(std::string /*path*/)
{
    NOT_IMPLEMENTED();
}

// Global
std::string SystemImplWin32::getErrorMessage()
{
    return strerror(m_lastErrno);
}

std::string SystemImplWin32::getEnv(std::string name)
{
    return getenv(name.c_str());
}

} // Unix

}
