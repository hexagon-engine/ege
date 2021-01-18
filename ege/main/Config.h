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

#pragma once

#include <iostream>
#include <stdlib.h>
#include <utility>

namespace EGE
{

[[noreturn]] void assertionFailed(const char* expr, const char* message, const char* file, unsigned line);

}

#define ASSERT_WITH_MESSAGE(expr, message) \
    if(!(expr)) EGE::assertionFailed(#expr, message, __FILE__, __LINE__)

#define ASSERT(expr) ASSERT_WITH_MESSAGE(expr, "Check failed")

#define CRASH_WITH_MESSAGE(message) \
    ASSERT_WITH_MESSAGE(false, message)

#define CRASH() CRASH_WITH_MESSAGE("Crash")

#define NOT_IMPLEMENTED(message) CRASH_WITH_MESSAGE("Not implemented: " message)

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
