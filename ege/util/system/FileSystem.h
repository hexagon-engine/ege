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

class FileInfo
{
public:

    enum class Type
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

    bool error()
    {
        return type == Type::Unknown || type == Type::NonExistent;
    }
    bool exists()
    {
        return type != Type::NonExistent;
    }

    Type type;
    size_t size;
};

FileInfo stat(std::string path);
std::string getWorkingDirectory();
bool setWorkingDirectory(std::string dir);
std::string readLink(std::string link);

}

}
