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

#include "SystemImplUnix.h"

#include <dirent.h>
#include <errno.h>
#include <limits.h>
#include <memory>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

namespace EGE
{

namespace System
{

std::unique_ptr<Internal::SystemImpl> g_impl = std::make_unique<Unix::SystemImplUnix>();

} // System

namespace Unix
{

// Time
time_t SystemImplUnix::unixTime()
{
    return time(nullptr);
}

System::ExactTime SystemImplUnix::exactTime()
{
    timespec _ts;
    if(clock_gettime(CLOCK_REALTIME, &_ts) < 0)
    {
        m_lastErrno = errno;
        return {};
    }

    return {_ts.tv_sec, _ts.tv_nsec};
}

// FileSystem
System::FileInfo SystemImplUnix::stat(std::string path)
{
    struct stat _stat;
    int rc = ::stat(path.c_str(), &_stat);

    System::FileInfo info;

    if(rc < 0)
    {
        m_lastErrno = errno;
        if(rc == ENOENT)
            info.type = System::FileType::NonExistent;
        else
            info.type = System::FileType::Unknown;

        return info;
    }

    // type
    switch(_stat.st_mode & S_IFMT)
    {
        case S_IFBLK:  info.type = System::FileType::BlockDevice;     break;
        case S_IFCHR:  info.type = System::FileType::CharacterDevice; break;
        case S_IFDIR:  info.type = System::FileType::Directory;       break;
        case S_IFIFO:  info.type = System::FileType::FIFO;            break;
        case S_IFLNK:  info.type = System::FileType::SymLink;         break;
        case S_IFREG:  info.type = System::FileType::File;            break;
        case S_IFSOCK: info.type = System::FileType::Socket;          break;
        default:       info.type = System::FileType::Unknown;         break;
    }

    // size
    info.size = _stat.st_size;

    // ignore other Unix-specific fields
    return info;
}

std::string SystemImplUnix::getWorkingDirectory()
{
    char buf[PATH_MAX];
    char* a = getcwd(buf, PATH_MAX);
    return a;
}

bool SystemImplUnix::setWorkingDirectory(std::string dir)
{
    int rc = chdir(dir.c_str());
    if(rc < 0)
        m_lastErrno = errno;
    return !rc;
}

std::string SystemImplUnix::readLink(std::string link)
{
    System::FileInfo info = stat(link);
    if(info.type != System::FileType::SymLink)
    {
        m_lastErrno = errno;
        return {};
    }

    char* buf = new char[info.size];
    int rc = readlink(link.c_str(), buf, info.size);
    if(rc < 0)
    {
        m_lastErrno = errno;
        return {};
    }

    std::string path = buf;
    delete[] buf;
    return path;
}

bool SystemImplUnix::testFileAccess(std::string path, System::FileOpenModeMask mode)
{
    int rc = access(path.c_str(), (int)mode);
    if(rc < 0)
        m_lastErrno = errno;
    return !rc;
}

bool SystemImplUnix::createDirectory(std::string path, System::FileMode mode)
{
    int rc = mkdir(path.c_str(), mode);
    if(rc < 0)
        m_lastErrno = errno;
    if(errno == EEXIST)
        rc = 0;
    return !rc;
}

bool SystemImplUnix::removeFile(std::string path)
{
    //std::cerr << "remove " << path << std::endl;
    int rc = remove(path.c_str());
    if(rc < 0)
        m_lastErrno = errno;
    if(errno == ENOENT)
        rc = 0;
    return !rc;
}

std::vector<std::string> SystemImplUnix::listFiles(std::string path)
{
    std::vector<std::string> names;
    std::string search_path = path;

    DIR* dir = opendir(path.c_str());
    if(dir)
    {
        dirent* entry;
        do
        {
            entry = readdir(dir);
            // read all (real) files in current folder
            // , delete '!' read other 2 default folder . and ..
            if(!entry || !entry->d_name)
                break;

            std::string fname = entry->d_name;
            if(fname != "." && fname != "..")
                names.push_back(path + "/" + std::string(entry->d_name));
        } while(true);

        closedir(dir);
        return names;
    }
    return {};
}

// Global
std::string SystemImplUnix::getErrorMessage()
{
    return strerror(m_lastErrno);
}

std::string SystemImplUnix::getEnv(std::string name)
{
    return getenv(name.c_str());
}

} // Unix

}
