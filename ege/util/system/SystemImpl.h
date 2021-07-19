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
    virtual ~SystemImpl() = default;

    // Time
    virtual time_t unixTime() = 0;
    virtual System::ExactTime exactTime() = 0;
    virtual void sleep(System::ExactTime) = 0;

    // FileSystem
    virtual System::FileInfo stat(std::string) = 0;
    virtual std::string getWorkingDirectory() = 0;
    virtual bool setWorkingDirectory(std::string) = 0;
    virtual std::string readLink(std::string) = 0;
    virtual bool testFileAccess(std::string, System::FileOpenModeMask) = 0;
    virtual bool createDirectory(std::string, System::FileMode) = 0;
    virtual bool removeFile(std::string) = 0;
    virtual std::vector<std::string> listFiles(std::string) = 0;

    // Global
    virtual std::string getErrorMessage() = 0;
    virtual std::string getEnv(std::string) = 0;

    virtual std::string className() { return "EGE::Internal::SystemImpl"; }
};

} // Internal

namespace System
{

Internal::SystemImpl& impl();

} // System

}
