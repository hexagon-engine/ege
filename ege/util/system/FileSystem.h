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

#include <string>
#include <vector>

namespace EGE
{

namespace System
{

enum class FileType
{
    Directory,
    File,

    // >>> unix-specific
    BlockDevice,
    CharacterDevice,
    FIFO,
    Socket,
    SymLink,
    // <<<

    Unknown,
    NonExistent
};

class FileInfo
{
public:
    bool error()
    {
        return type == FileType::Unknown || type == FileType::NonExistent;
    }
    bool exists()
    {
        return type != FileType::NonExistent;
    }

    FileType type;
    size_t size;
};

namespace FileOpenMode
{
    const int
        None = 0,
        Execute = 1,
        Write = 2,
        Read = 4;
};
typedef int FileOpenModeMask;
typedef int FileMode;

#define FMODE(s,u,g,o) (o | (g << 3) | (u << 6) | (s << 9))

FileInfo stat(std::string path);
std::string getWorkingDirectory();
bool setWorkingDirectory(std::string dir);
std::string readLink(std::string link);
bool testFileAccess(std::string path, FileOpenModeMask mode = FileOpenMode::None);
bool createDirectory(std::string path, System::FileMode mode = FMODE(0,7,5,0));
bool createPath(std::string path, System::FileMode mode = FMODE(0,7,5,0));
bool removeFile(std::string path);
bool removePath(std::string path);
std::vector<std::string> listFiles(std::string path);

}

}
