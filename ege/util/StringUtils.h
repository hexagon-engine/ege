/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#pragma once

#include <cxxabi.h>
#include <string>

namespace EGE
{
    template<typename T>
    std::string typeName()
    {
        int status;
        std::string tname = typeid(T).name();
        char *demangled = abi::__cxa_demangle(tname.c_str(), nullptr, nullptr, &status);
        if(status == 0)
        {
            tname = demangled;
            std::free(demangled);
        }
        return tname;
    }

    template<typename T>
    inline std::string display(T& first)
    {
        return std::to_string(first);
    }

    template<typename T, typename... Args>
    inline std::string display(T& first, Args&... args)
    {
        return std::to_string(first) + ", " + display(args...);
    }
}

using EGE::display;
using EGE::typeName;
