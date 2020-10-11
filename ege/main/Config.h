/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#pragma once

#include <iostream>
#include <stdlib.h>
#include <utility>

extern "C"
[[noreturn]] void _ege_assertion_failed(const char* expr, const char* file, unsigned line);

#define ASSERT(expr) \
    if(!(expr)) _ege_assertion_failed(#expr, __FILE__, __LINE__)

#define CRASH() ASSERT(false)

#define EGE_ENUM_YES_NO(X) \
    enum class X : bool \
    { \
        Yes, \
        No \
    }

#define EGE_SINGLETON_VA(clazz,...) \
    static clazz& instance() \
    { \
        static clazz inst(__VA_ARGS__); \
        return inst; \
    }

#define EGE_SINGLETON(clazz) \
    static clazz& instance() \
    { \
        static clazz inst; \
        return inst; \
    }

#define DBG(run,txt) \
    if constexpr(run) \
    { \
        std::cerr << "DBG  " << (txt) << std::endl; \
    }

#define DUMP(run,var) \
    if constexpr(run) \
    { \
        std::cerr << "DUMP  '" << #var << "' = [" << (var) << "]" << std::endl; \
    }

#define instanceof(pointer, clazz) \
    (dynamic_cast<clazz*>(pointer) != nullptr)
