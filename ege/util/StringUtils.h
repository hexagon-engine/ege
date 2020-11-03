/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#pragma once

#include <cxxabi.h>

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

    inline std::string display()
    {
        return "";
    }

    template<typename T, typename... Args>
    inline std::string display(T& first, Args&... args)
    {
        return toString(first) + ", " + display(args...);
    }
}

using EGE::display;
using EGE::typeName;
