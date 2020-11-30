/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
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
