/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#include "SystemImplUnix.h"

#include <errno.h>
#include <limits.h>
#include <memory>
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
        return {};

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
        if(rc == ENOENT)
            info.type = System::FileInfo::Type::NonExistent;
        else
            info.type = System::FileInfo::Type::Unknown;

        return info;
    }

    // type
    switch(_stat.st_mode & S_IFMT)
    {
        case S_IFBLK:  info.type = System::FileInfo::Type::BlockDevice;     break;
        case S_IFCHR:  info.type = System::FileInfo::Type::CharacterDevice; break;
        case S_IFDIR:  info.type = System::FileInfo::Type::Directory;       break;
        case S_IFIFO:  info.type = System::FileInfo::Type::FIFO;            break;
        case S_IFLNK:  info.type = System::FileInfo::Type::SymLink;         break;
        case S_IFREG:  info.type = System::FileInfo::Type::File;            break;
        case S_IFSOCK: info.type = System::FileInfo::Type::Socket;          break;
        default:       info.type = System::FileInfo::Type::Unknown;         break;
    }

    // size
    info.size = _stat.st_size;

    // ignore other Unix-specific fields
    return info;
}

std::string SystemImplUnix::getWorkingDirectory()
{
    char buf[PATH_MAX];
    getcwd(buf, PATH_MAX);
    return buf;
}

bool SystemImplUnix::setWorkingDirectory(std::string dir)
{
    return chdir(dir.c_str()) >= 0;
}

std::string SystemImplUnix::readLink(std::string link)
{
    System::FileInfo info = stat(link);
    if(info.type != System::FileInfo::Type::SymLink)
        return {};

    char* buf = new char[info.size];
    int rc = readlink(link.c_str(), buf, info.size);
    if(rc < 0)
        return {};

    std::string path = buf;
    delete[] buf;
    return path;
}

} // Unix

}
