/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
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
