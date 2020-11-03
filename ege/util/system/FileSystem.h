/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#pragma once

#include <string>

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

FileInfo stat(std::string path);
std::string getWorkingDirectory();
bool setWorkingDirectory(std::string dir);
std::string readLink(std::string link);

}

}
