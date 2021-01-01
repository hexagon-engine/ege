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

#include "FileSystem.h"

#include "SystemImpl.h"

#include <ege/util/Types.h>

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

bool testFileAccess(std::string path, FileOpenModeMask mode)
{
    return impl().testFileAccess(path, mode);
}

bool createDirectory(std::string path, FileMode mode)
{
    return impl().createDirectory(path, mode);
}

bool createPath(std::string path, FileMode mode)
{
    for(EGE::Size s = 0; s < path.size() + 1; s++)
    {
        if(path[s] == '/' || s == path.size())
        {
            if(!createDirectory(path.substr(0, s), mode))
            {
                return false;
            }
        }
    }
    return true;
}

bool removeFile(std::string path)
{
    return impl().removeFile(path);
}

bool removePath(std::string path)
{
    if(stat(path).type == FileType::Directory)
    {
        // Remove all sub-directories and files
        std::vector<std::string> files = listFiles(path);
        for(auto& file: files)
        {
            //std::cerr << "rp " << file << std::endl;
            // Remove sub-directories
            if(!removePath(file))
            {
                std::cerr << "err: " << impl().getErrorMessage() << std::endl;
                return false;
            }
        }
    }

    return removeFile(path);
}

std::vector<std::string> listFiles(std::string path)
{
    return impl().listFiles(path);
}

} // System

}
