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

#include <array>
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

namespace Internal
{

// https://bitwizeshift.github.io/posts/2021/03/09/getting-an-unmangled-type-name-at-compile-time/
template<std::size_t... Idxs>
constexpr auto _substringArray(std::string_view str, std::index_sequence<Idxs...>)
{
    return std::array{str[Idxs]..., '\n'};
}

template<typename T>
constexpr auto _typeNameArray()
{
#if defined(__clang__)
    constexpr auto prefix   = std::string_view{"[T = "};
    constexpr auto suffix   = std::string_view{"]"};
    constexpr auto function = std::string_view{__PRETTY_FUNCTION__};
#elif defined(__GNUC__)
    constexpr auto prefix   = std::string_view{"with T = "};
    constexpr auto suffix   = std::string_view{"]"};
    constexpr auto function = std::string_view{__PRETTY_FUNCTION__};
#elif defined(_MSC_VER)
    constexpr auto prefix   = std::string_view{"type_name_array<"};
    constexpr auto suffix   = std::string_view{">(void)"};
    constexpr auto function = std::string_view{__FUNCSIG__};
#else
#   error Unsupported compiler for _typeNameArray
#endif

    constexpr auto start = function.find(prefix) + prefix.size();
    constexpr auto end = function.rfind(suffix);

    static_assert(start < end);

    constexpr auto name = function.substr(start, (end - start));
    return _substringArray(name, std::make_index_sequence<name.size()>{});
}

template<typename T>
struct _TypeNameHolder
{
    static inline constexpr auto value = _typeNameArray<T>();
};

}

// https://bitwizeshift.github.io/posts/2021/03/09/getting-an-unmangled-type-name-at-compile-time/
template<typename T>
constexpr auto typeof_() -> std::string_view
{
    constexpr auto& value = Internal::_TypeNameHolder<T>::value;
    return std::string_view{value.data(), value.size()};
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
