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

#include "SystemImpl.h"

namespace EGE
{

namespace Windows
{

class SystemImplWin32 final : public Internal::SystemImpl
{
public:
    // Time
    virtual time_t unixTime() override;
    virtual System::ExactTime exactTime() override;
    virtual void sleep(System::ExactTime) override;

    // FileSystem
    virtual System::FileInfo stat(std::string path) override;
    virtual std::string getWorkingDirectory() override;
    virtual bool setWorkingDirectory(std::string dir) override;
    virtual std::string readLink(std::string link) override;
    virtual bool testFileAccess(std::string path, System::FileOpenModeMask mode) override;
    virtual bool createDirectory(std::string path, System::FileMode mode) override;
    virtual bool removeFile(std::string path) override;
    virtual std::vector<std::string> listFiles(std::string path) override;

    // Global
    virtual std::string getErrorMessage() override;
    virtual std::string getEnv(std::string) override;

    virtual std::string className() override { return "EGE::Windows::SystemImplWin32"; }

private:
    int m_lastErrno = 0;
};

} // Windows

}
