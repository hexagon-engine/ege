/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#pragma once

#include <stdlib.h>

void _ege_assertion_failed(const char* expr, const char* file, size_t line);

#define ASSERT(expr) \
    if(!(expr)) \
    { \
        _ege_assertion_failed(#expr, __FILE__, __LINE__); \
    }

#define EGE_ENUM_YES_NO(X) \
    enum class X : bool \
    { \
        Yes, \
        No \
    };

#define DUMP(run,var) \
    if constexpr(run) \
    { \
        std::cerr << #var << "= " << (var) << std::endl; \
    }

#define DBG(run,txt) \
    if constexpr(run) \
    { \
        std::cerr << "0004 EGE/main: debug: " << (txt) << std::endl; \
    }

